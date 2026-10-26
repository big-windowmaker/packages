/*
 * Testes da libwdm do wdm (lista de sessões de src/libwdm/sessions.c,
 * estado do pam_faillock de src/libwdm/faillock.c e WDMUnsetEnv), rodados
 * no check() do PKGBUILD:
 *   cc -Iinclude $(pkg-config --cflags WINGs) test-sessions.c bin/libwdm.a \
 *      $(pkg-config --libs WINGs) -o test-sessions
 *   ./test-sessions <diretório temporário vazio>
 */
#include <wdmsessions.h>
#include <wdmfaillock.h>
#include <wdmlib.h>

#include <stdint.h>
#include <time.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

static int failures;

#define CHECK(cond) do { if (!(cond)) { \
	fprintf(stderr, "FALHOU %s:%d: %s\n", __FILE__, __LINE__, #cond); \
	failures++; } } while (0)

static void put(const char *dir, const char *name, const char *text)
{
	char path[1024];
	FILE *f;

	snprintf(path, sizeof(path), "%s/%s", dir, name);
	f = fopen(path, "w");
	if (!f) {
		perror(path);
		exit(2);
	}
	fputs(text, f);
	fclose(f);
}

/* registro do pam_faillock (faillock.h do Linux-PAM) */
struct tally {
	char source[52];
	uint16_t reserved;
	uint16_t status;
	uint64_t time;
};

static void put_tally(const char *dir, const char *user, const long *ago,
		      int n, time_t now, int valid)
{
	char path[1024];
	struct tally t;
	FILE *f;
	int i;

	snprintf(path, sizeof(path), "%s/%s", dir, user);
	f = fopen(path, "w");
	if (!f) {
		perror(path);
		exit(2);
	}
	for (i = 0; i < n; i++) {
		memset(&t, 0, sizeof(t));
		snprintf(t.source, sizeof(t.source), "tty7");
		t.status = valid ? 0x1 | 0x4 : 0;
		t.time = (uint64_t)(now - ago[i]);
		fwrite(&t, sizeof(t), 1, f);
	}
	fclose(f);
}

static void check_split(const char *exec, const char **expected)
{
	char **argv = WDMSplitExec(exec);
	int i;

	if (!expected) {
		CHECK(argv == NULL);
		WDMFreeArgv(argv);
		return;
	}
	CHECK(argv != NULL);
	if (!argv)
		return;
	for (i = 0; expected[i]; i++) {
		CHECK(argv[i] != NULL);
		if (!argv[i])
			break;
		if (strcmp(argv[i], expected[i]) != 0) {
			fprintf(stderr, "  Exec=%s: argv[%d]=\"%s\", esperado \"%s\"\n",
				exec, i, argv[i], expected[i]);
			failures++;
		}
	}
	CHECK(argv[i] == NULL);
	WDMFreeArgv(argv);
}

int main(int argc, char **argv)
{
	char x1[1024], x2[1024], w1[1024], state[1024], dirs[2100], key[300];
	WDMSessionList *l;
	WDMSession *s;
	char *loaded;
	int i;

	if (argc != 2) {
		fprintf(stderr, "uso: %s <diretório vazio>\n", argv[0]);
		return 2;
	}
	snprintf(x1, sizeof(x1), "%s/x1", argv[1]);
	snprintf(x2, sizeof(x2), "%s/x2", argv[1]);
	snprintf(w1, sizeof(w1), "%s/w1", argv[1]);
	snprintf(state, sizeof(state), "%s/state", argv[1]);
	mkdir(x1, 0755);
	mkdir(x2, 0755);
	mkdir(w1, 0755);

	put(x1, "big-wm.desktop",
	    "[Desktop Entry]\nType=XSession\nName=Big WindowMaker (X11)\n"
	    "Name[pt_BR]=Big WindowMaker (X11)\nExec=/usr/bin/startbigwm\n"
	    "TryExec=/bin/sh\nDesktopNames=WindowMaker;GNUstep\n");
	put(x1, "wmaker.desktop",
	    "# comentário\n[Desktop Entry]\nName=Window Maker\nName[de]=Fenstermacher\n"
	    "Exec=wmaker\nTryExec=sh\n[Desktop Action foo]\nName=Ignorar\n");
	put(x1, "hidden.desktop", "[Desktop Entry]\nName=Oculta\nExec=x\nHidden=true\n");
	put(x1, "nodisplay.desktop", "[Desktop Entry]\nName=Oculta\nExec=x\nNoDisplay=true\n");
	put(x1, "notry.desktop", "[Desktop Entry]\nName=Sem TryExec\nExec=x\nTryExec=/nao/existe\n");
	put(x1, "noexec.desktop", "[Desktop Entry]\nName=Sem Exec\n");
	put(x1, "badquote.desktop", "[Desktop Entry]\nName=Aspas\nExec=\"foo\n");
	put(x1, "bad id.desktop", "[Desktop Entry]\nName=Id inválido\nExec=x\n");
	put(x1, "notes.txt", "não é .desktop\n");
	/* mesmo id em x2: x1 ganha */
	put(x2, "wmaker.desktop", "[Desktop Entry]\nName=Duplicada\nExec=outro\n");
	put(x2, "plasmax11.desktop", "[Desktop Entry]\nName=Plasma (X11)\nExec=startplasma-x11\n");
	put(w1, "big-wm-wayland.desktop",
	    "[Desktop Entry]\nName=Big WindowMaker (Wayland)\n"
	    "Exec=/usr/bin/startbigwm-wayland\nDesktopNames=wlmaker\n");
	put(w1, "big-wm.desktop", "[Desktop Entry]\nName=Mesmo id\nExec=wl\n");
	put(w1, "labwc.desktop", "[Desktop Entry]\nName=labwc\nName[pt]=labwc pt\nExec=labwc\n");

	snprintf(dirs, sizeof(dirs), "%s:%s:/nao/existe:relativo", x1, x2);
	l = WDMReadSessions(dirs, w1, "pt_BR.UTF-8@foo");
	for (i = 0; i < l->count; i++)
		printf("  %-8s %-16s %-30s %s\n",
		       l->items[i].type == WDM_SESSION_WAYLAND ? "wayland" : "x11",
		       l->items[i].id, l->items[i].label, l->items[i].exec);
	CHECK(l->count == 6);

	s = WDMFindSession(l, "x11/big-wm");
	CHECK(s && s->type == WDM_SESSION_X11 && strcmp(s->label, "Big WindowMaker (X11)") == 0);
	CHECK(s && strcmp(s->desktopNames, "WindowMaker;GNUstep") == 0);
	s = WDMFindSession(l, "wayland/big-wm");
	CHECK(s && s->type == WDM_SESSION_WAYLAND && strcmp(s->label, "Mesmo id (Wayland)") == 0);
	CHECK(WDMFindSession(l, "big-wm") == NULL);	/* ambíguo sem tipo */
	s = WDMFindSession(l, "big-wm-wayland");
	CHECK(s && strcmp(s->label, "Big WindowMaker (Wayland)") == 0);
	s = WDMFindSession(l, "x11/wmaker");
	CHECK(s && strcmp(s->exec, "wmaker") == 0 && strcmp(s->label, "Window Maker (X11)") == 0);
	s = WDMFindSession(l, "wayland/labwc");
	CHECK(s && strcmp(s->label, "labwc pt (Wayland)") == 0);	/* Name[pt] */
	s = WDMFindSession(l, "x11/plasmax11");
	CHECK(s && strcmp(s->label, "Plasma (X11)") == 0);
	CHECK(WDMFindSession(l, "x11/hidden") == NULL);
	CHECK(WDMFindSession(l, "x11/nodisplay") == NULL);
	CHECK(WDMFindSession(l, "x11/notry") == NULL);
	CHECK(WDMFindSession(l, "x11/noexec") == NULL);
	CHECK(WDMFindSession(l, "x11/badquote") == NULL);
	CHECK(WDMFindSession(l, "x11/../x1/wmaker") == NULL);
	CHECK(WDMFindSession(l, "x11/") == NULL);
	CHECK(WDMFindSession(l, "tty/wmaker") == NULL);
	CHECK(WDMFindSession(l, NULL) == NULL);
	/* ordenada pelo rótulo */
	for (i = 1; i < l->count; i++)
		CHECK(strcasecmp(l->items[i - 1].label, l->items[i].label) <= 0);
	s = WDMFindSession(l, "x11/big-wm");
	CHECK(s && strcmp(WDMSessionKey(s, key, sizeof(key)), "x11/big-wm") == 0);
	WDMFreeSessions(l);

	/* localidade C: Name sem tradução */
	l = WDMReadSessions(x1, "", "C");
	s = WDMFindSession(l, "wmaker");
	CHECK(l->count == 2 && s && strcmp(s->label, "Window Maker (X11)") == 0);
	WDMFreeSessions(l);
	l = WDMReadSessions(x1, NULL, "de_DE.UTF-8");
	s = WDMFindSession(l, "wmaker");
	CHECK(s && strcmp(s->label, "Fenstermacher (X11)") == 0);
	WDMFreeSessions(l);
	l = WDMReadSessions("", "", NULL);
	CHECK(l && l->count == 0);
	WDMFreeSessions(l);

	/* Exec */
	check_split("startbigwm", (const char *[]){ "startbigwm", NULL });
	check_split("  wlmaker  --start_xwayland  ", (const char *[]){ "wlmaker", "--start_xwayland", NULL });
	check_split("env \"A B\" c", (const char *[]){ "env", "A B", "c", NULL });
	check_split("sh -c \"echo \\\"x\\\" \\$HOME \\\\\"", (const char *[]){ "sh", "-c", "echo \"x\" $HOME \\", NULL });
	check_split("foo %f %U bar", (const char *[]){ "foo", "bar", NULL });
	check_split("foo 100%%", (const char *[]){ "foo", "100%", NULL });
	check_split("foo \"\"", (const char *[]){ "foo", "", NULL });
	check_split("", NULL);
	check_split("   ", NULL);
	check_split("%f", NULL);
	check_split("\"unterminated", NULL);

	/* ids e nomes de usuário */
	CHECK(WDMValidSessionId("big-wm-wayland"));
	CHECK(WDMValidSessionId("org.kde.plasma+x11@1"));
	CHECK(!WDMValidSessionId("../etc"));
	CHECK(!WDMValidSessionId("a/b"));
	CHECK(!WDMValidSessionId(".hidden"));
	CHECK(!WDMValidSessionId(""));
	CHECK(WDMValidUserName("maria"));
	CHECK(WDMValidUserName("joao.silva_2"));
	CHECK(WDMValidUserName("machine$"));
	CHECK(!WDMValidUserName("../root"));
	CHECK(!WDMValidUserName("a/b"));
	CHECK(!WDMValidUserName("-rf"));
	CHECK(!WDMValidUserName("a$b"));

	/* sessão lembrada */
	CHECK(WDMLoadUserSession(state, "maria") == NULL);
	CHECK(WDMSaveUserSession(state, "maria", "wayland/big-wm-wayland"));
	loaded = WDMLoadUserSession(state, "maria");
	CHECK(loaded && strcmp(loaded, "wayland/big-wm-wayland") == 0);
	free(loaded);
	CHECK(WDMSaveUserSession(state, "maria", "x11/big-wm"));
	loaded = WDMLoadUserSession(state, "maria");
	CHECK(loaded && strcmp(loaded, "x11/big-wm") == 0);
	free(loaded);
	CHECK(WDMSaveUserSession(state, "maria", NULL));	/* esquece */
	CHECK(WDMLoadUserSession(state, "maria") == NULL);
	CHECK(WDMSaveUserSession(state, "maria", NULL));	/* já esquecida */
	CHECK(!WDMSaveUserSession(state, "../maria", "x11/big-wm"));
	CHECK(WDMLoadUserSession(state, "../maria") == NULL);

	/* pam_faillock */
	{
		char fl[1024], conf[1100], conf2[1100], conf3[1100];
		time_t now = time(NULL);
		const long recent[] = { 30, 20, 10 };
		const long spread[] = { 2000, 1990, 5 };
		const long old[] = { 720, 710, 700 };

		CHECK(sizeof(struct tally) == 64);
		snprintf(fl, sizeof(fl), "%s/faillock", argv[1]);
		mkdir(fl, 0755);
		snprintf(conf, sizeof(conf), "%s/faillock.conf", argv[1]);
		put(argv[1], "faillock.conf", "# padrão do Arch\n deny = 3\nunlock_time=600\n");
		snprintf(conf2, sizeof(conf2), "%s/faillock-never.conf", argv[1]);
		put(argv[1], "faillock-never.conf", "deny = 3\nunlock_time = never\neven_deny_root\n");
		snprintf(conf3, sizeof(conf3), "%s/faillock-dir.conf", argv[1]);
		{
			char text[1200];
			snprintf(text, sizeof(text), "dir = %s\ndeny=3\n", fl);
			put(argv[1], "faillock-dir.conf", text);
		}
		CHECK(WDMFaillockMinutesLeft("maria", 0, conf, fl, now) == 0);	/* sem arquivo */
		put_tally(fl, "maria", recent, 2, now, 1);
		CHECK(WDMFaillockMinutesLeft("maria", 0, conf, fl, now) == 0);	/* 2 < deny */
		put_tally(fl, "maria", recent, 3, now, 1);
		CHECK(WDMFaillockMinutesLeft("maria", 0, conf, fl, now) == 10);	/* 590 s */
		CHECK(WDMFaillockMinutesLeft("maria", 0, conf3, NULL, now) == 10);	/* dir do .conf */
		CHECK(WDMFaillockMinutesLeft("maria", 0, conf, fl, now + 589) == 1);
		CHECK(WDMFaillockMinutesLeft("maria", 0, conf, fl, now + 600) == 0);	/* expirou */
		CHECK(WDMFaillockMinutesLeft("maria", 0, conf2, fl, now + 99999) == -1);	/* never */
		CHECK(WDMFaillockMinutesLeft("maria", 1, conf, fl, now) == 0);	/* root sem even_deny_root */
		CHECK(WDMFaillockMinutesLeft("maria", 1, conf2, fl, now) == -1);
		CHECK(WDMFaillockMinutesLeft("../maria", 0, conf, fl, now) == 0);
		put_tally(fl, "maria", spread, 3, now, 1);
		CHECK(WDMFaillockMinutesLeft("maria", 0, conf, fl, now) == 0);	/* fora do fail_interval */
		put_tally(fl, "maria", old, 3, now, 1);
		CHECK(WDMFaillockMinutesLeft("maria", 0, conf, fl, now) == 0);	/* já desbloqueou */
		put_tally(fl, "maria", recent, 3, now, 0);
		CHECK(WDMFaillockMinutesLeft("maria", 0, conf, fl, now) == 0);	/* registros inválidos */
	}

	/* WDMUnsetEnv: variável ausente não pode apagar o ambiente */
	{
		char **env = NULL;

		env = WDMSetEnv(env, "A", "1");
		env = WDMSetEnv(env, "DISPLAY", ":0");
		env = WDMSetEnv(env, "B", "2");
		env = WDMUnsetEnv(env, "XAUTHORITY");
		CHECK(env != NULL);
		env = WDMUnsetEnv(env, "DISPLAY");
		CHECK(env != NULL && WDMGetEnv(env, "DISPLAY") == NULL);
		CHECK(env != NULL && WDMGetEnv(env, "A") && strcmp(WDMGetEnv(env, "A"), "1") == 0);
		CHECK(env != NULL && WDMGetEnv(env, "B") && strcmp(WDMGetEnv(env, "B"), "2") == 0);
		env = WDMUnsetEnv(env, "DISPLAY");
		CHECK(env != NULL && WDMGetEnv(env, "B") != NULL);
		WDMFreeEnv(env);
	}

	if (failures) {
		fprintf(stderr, "%d verificação(ões) falharam\n", failures);
		return 1;
	}
	printf("test-sessions: ok\n");
	return 0;
}
