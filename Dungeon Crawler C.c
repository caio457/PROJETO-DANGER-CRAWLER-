#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifdef _WIN32
#define CLEAR "cls"
#else
#define CLEAR "clear"
#endif

#define MAX 25
#define VILLAGE 0
#define FLOOR1 1
#define FLOOR2 2
#define FLOOR3 3

typedef struct {
    int x, y;
    int startX, startY;
    char dir;
    int lives;
    int keys;
    int weapon; // 1 espada, 2 arco, 3 cajado
} Player;

typedef struct {
    int x, y;
    int alive;
    char type; // X, Y ou Z
} Enemy;

typedef struct {
    char map[MAX][MAX];
    char original[MAX][MAX];
    int rows, cols;
    int floor;
    int monsterCount;
    Enemy monsters[20];
    int bossHP;
    int buttonUsed;
} Level;

Player p;
Level lvl;
int running = 1;

void pauseGame() {
    printf("\nPressione ENTER para continuar...");
    getchar();
}

void clearScreen() {
    system(CLEAR);
}

void copyMap(char dest[MAX][MAX], const char *src[], int rows, int cols) {
    int i;
for (i = 0; i < rows; i++) {
        int j;
for (j = 0; j < cols; j++) {
            dest[i][j] = src[i][j];
        }
    }
}

void findPlayerStart() {
    int i;
    int j;

    for (i = 0; i < lvl.rows; i++) {
        for (j = 0; j < lvl.cols; j++) {
            if (lvl.map[i][j] == '^' || lvl.map[i][j] == 'v' || lvl.map[i][j] == '<' || lvl.map[i][j] == '>') {
                p.x = i;
                p.y = j;
                p.startX = i;
                p.startY = j;
                p.dir = lvl.map[i][j];
                lvl.map[i][j] = ' ';
            }
        }
    }
}

void findMonsters() {
    int i;
    int j;

    lvl.monsterCount = 0;
    lvl.bossHP = 6;
    for (i = 0; i < lvl.rows; i++) {
        for (j = 0; j < lvl.cols; j++) {
            if (lvl.map[i][j] == 'X' || lvl.map[i][j] == 'Y' || lvl.map[i][j] == 'Z') {
                lvl.monsters[lvl.monsterCount].x = i;
                lvl.monsters[lvl.monsterCount].y = j;
                lvl.monsters[lvl.monsterCount].alive = 1;
                lvl.monsters[lvl.monsterCount].type = lvl.map[i][j];
                lvl.monsterCount++;
                lvl.map[i][j] = ' ';
            }
        }
    }
}

void loadLevel(int floor) {
    lvl.floor = floor;
    lvl.buttonUsed = 0;
    p.keys = 0;

    if (floor == VILLAGE) {
        const char *m[] = {
            "**********",
            "*^   N  *",
            "*        *",
            "*   ***  *",
            "*        *",
            "*        *",
            "*  Bem   *",
            "*        *",
            "*      L *",
            "**********"
        };
        lvl.rows = 10; lvl.cols = 10;
        copyMap(lvl.map, m, 10, 10);
    } else if (floor == FLOOR1) {
        const char *m[] = {
            "**********",
            "*^   @   *",
            "* *** ** *",
            "* k   D  *",
            "* *** ** *",
            "*        *",
            "*   kk   *",
            "*        *",
            "*      L *",
            "**********"
        };
        lvl.rows = 10; lvl.cols = 10;
        copyMap(lvl.map, m, 10, 10);
    } else if (floor == FLOOR2) {
        const char *m[] = {
            "***************",
            "*^   @     X  *",
            "* *** *****   *",
            "*   #    k    *",
            "* *** *** *** *",
            "*   O   D     *",
            "* ***** *** ***",
            "*     @       *",
            "*** ***** *** *",
            "*      #      *",
            "* *** *** *** *",
            "*       D     *",
            "*   k      X  *",
            "*          L  *",
            "***************"
        };
        lvl.rows = 15; lvl.cols = 15;
        copyMap(lvl.map, m, 15, 15);
    } else if (floor == FLOOR3) {
        const char *m[] = {
            "*************************",
            "*^   @      k       Y   *",
            "* ***** *********** *** *",
            "*     #      D          *",
            "*** ***** ******* ***** *",
            "*   O       @       #   *",
            "* ***** *** ***** ***** *",
            "*     k       Y         *",
            "* ********* ***** ***** *",
            "*     D       @         *",
            "***** ***** ********* ***",
            "*     #         k       *",
            "* *** ***** ***** ***** *",
            "*   Y       D           *",
            "* ***** ********* ***** *",
            "*       #       O       *",
            "* ********* ***** ***** *",
            "*       k         #     *",
            "***** ***** ********* ***",
            "*           Y           *",
            "* ***** *********** *****",
            "*         Z             *",
            "* ***** *********** *** *",
            "*                    L  *",
            "*************************"
        };
        lvl.rows = 25; lvl.cols = 25;
        copyMap(lvl.map, m, 25, 25);
    }

    memcpy(lvl.original, lvl.map, sizeof(lvl.map));
    findPlayerStart();
    findMonsters();
}

void resetCurrentLevel() {
    int floor = lvl.floor;
    int weapon = p.weapon;
    int lives = p.lives;
    loadLevel(floor);
    p.weapon = weapon;
    p.lives = lives;
}

int isInside(int x, int y) {
    return x >= 0 && x < lvl.rows && y >= 0 && y < lvl.cols;
}

int isEnemyAt(int x, int y) {
    int i;

    for (i = 0; i < lvl.monsterCount; i++) {
        if (lvl.monsters[i].alive && lvl.monsters[i].x == x && lvl.monsters[i].y == y) return i;
    }
    return -1;
}

void drawMap() {
    int i;
    int j;

    clearScreen();
    printf("DUNGEON CRAWLER | Vidas: %d | Chaves: %d | Arma: ", p.lives, p.keys);
    if (p.weapon == 1) printf("Espada\n");
    else if (p.weapon == 2) printf("Arco\n");
    else if (p.weapon == 3) printf("Cajado\n");
    else printf("Nenhuma\n");

    if (lvl.floor == VILLAGE) printf("Local: Vila\n\n");
    else printf("Andar: %d\n\n", lvl.floor);

    for (i = 0; i < lvl.rows; i++) {
        for (j = 0; j < lvl.cols; j++) {
            int e = isEnemyAt(i, j);
            if (p.x == i && p.y == j) printf("%c", p.dir);
            else if (e != -1) printf("%c", lvl.monsters[e].type);
            else printf("%c", lvl.map[i][j]);
        }
        printf("\n");
    }
    printf("\nComandos: w/a/s/d mover | i interagir | o atacar | q voltar ao menu\n");
}

void directionDelta(int *dx, int *dy) {
    *dx = 0; *dy = 0;
    if (p.dir == '^') *dx = -1;
    else if (p.dir == 'v') *dx = 1;
    else if (p.dir == '<') *dy = -1;
    else if (p.dir == '>') *dy = 1;
}

void loseLife() {
    p.lives--;
    if (p.lives <= 0) {
        clearScreen();
        printf("====================\n");
        printf("      GAME OVER     \n");
        printf("====================\n");
        pauseGame();
        loadLevel(VILLAGE);
        p.lives = 3;
        p.weapon = 0;
    } else {
        printf("\nVoce perdeu uma vida! A fase sera reiniciada.\n");
        pauseGame();
        resetCurrentLevel();
    }
}

void chooseWeapon() {
    int op;
    clearScreen();
    printf("NPC: Antes de entrar na masmorra, escolha sua arma:\n\n");
    printf("1 - Espada: ataca area 3x2 na frente\n");
    printf("2 - Arco: ataca 4 casas em linha reta\n");
    printf("3 - Cajado: ataca as 8 casas ao redor\n");
    printf("Escolha: ");
    scanf("%d", &op);
    getchar();
    if (op < 1 || op > 3) op = 1;
    p.weapon = op;
    printf("\nArma escolhida!\n");
    pauseGame();
}

void interact() {
    int i;
    int j;

    int dx, dy;
    directionDelta(&dx, &dy);
    int nx = p.x + dx, ny = p.y + dy;
    if (!isInside(nx, ny)) return;

    char c = lvl.map[nx][ny];
    if (c == 'N') {
        chooseWeapon();
    } else if (c == '@') {
        p.keys++;
        lvl.map[nx][ny] = ' ';
        printf("\nVoce pegou uma chave!\n");
        pauseGame();
    } else if (c == 'D') {
        if (p.keys > 0) {
            p.keys--;
            lvl.map[nx][ny] = '=';
            printf("\nVoce abriu uma porta!\n");
        } else {
            printf("\nVoce precisa de uma chave.\n");
        }
        pauseGame();
    } else if (c == 'O') {
        if (!lvl.buttonUsed) {
            lvl.buttonUsed = 1;
            printf("\nBotao ativado! Uma passagem secreta foi aberta.\n");
            for (i = 0; i < lvl.rows; i++) {
                for (j = 0; j < lvl.cols; j++) {
                    if (lvl.map[i][j] == 'B') lvl.map[i][j] = ' ';
                }
            }
            if (lvl.floor == FLOOR2) lvl.map[6][13] = ' ';
            if (lvl.floor == FLOOR3) lvl.map[20][23] = ' ';
        } else {
            printf("\nEsse botao ja foi usado.\n");
        }
        pauseGame();
    } else {
        printf("\nNao ha nada para interagir.\n");
        pauseGame();
    }
}

void damageCell(int x, int y) {
    if (!isInside(x, y)) return;
    int e = isEnemyAt(x, y);
    if (e != -1) {
        if (lvl.monsters[e].type == 'Z') {
            lvl.bossHP--;
            printf("Voce acertou o boss! HP restante: %d\n", lvl.bossHP);
            if (lvl.bossHP <= 0) {
                lvl.monsters[e].alive = 0;
                clearScreen();
                printf("============================\n");
                printf("          VITORIA!          \n");
                printf("============================\n");
                printf("Voce derrotou o guardiao da masmorra e libertou a vila.\n");
                pauseGame();
                loadLevel(VILLAGE);
                p.lives = 3;
                p.weapon = 0;
            }
        } else {
            lvl.monsters[e].alive = 0;
            printf("Monstro derrotado!\n");
        }
    }
    if (lvl.map[x][y] == 'k') {
        lvl.map[x][y] = ' ';
        printf("Caixa destruida!\n");
    }
}

void attack() {
    int i;
    int j;
    int side;

    if (p.weapon == 0) {
        printf("\nVoce ainda nao escolheu uma arma. Fale com o NPC na vila.\n");
        pauseGame();
        return;
    }

    int dx, dy;
    directionDelta(&dx, &dy);
    printf("\nAtaque realizado!\n");

    if (p.weapon == 1) { // espada 3x2
        int depth;
for (depth = 1; depth <= 2; depth++) {
            int side;
for (side = -1; side <= 1; side++) {
                int x = p.x + dx * depth;
                int y = p.y + dy * depth;
                if (dx != 0) y += side;
                else x += side;
                damageCell(x, y);
            }
        }
    } else if (p.weapon == 2) { // arco 4 em linha
        for (i = 1; i <= 4; i++) {
            damageCell(p.x + dx * i, p.y + dy * i);
        }
    } else if (p.weapon == 3) { // cajado 8 ao redor
        for (i = -1; i <= 1; i++) {
            for (j = -1; j <= 1; j++) {
                if (i != 0 || j != 0) damageCell(p.x + i, p.y + j);
            }
        }
    }
    pauseGame();
}

int canMoveTo(int x, int y) {
    if (!isInside(x, y)) return 0;
    char c = lvl.map[x][y];
    if (c == '*' || c == 'D' || c == 'k' || c == 'N' || c == 'O') return 0;
    return 1;
}

void nextLevel() {
    if (lvl.floor == VILLAGE) loadLevel(FLOOR1);
    else if (lvl.floor == FLOOR1) loadLevel(FLOOR2);
    else if (lvl.floor == FLOOR2) loadLevel(FLOOR3);
    else if (lvl.floor == FLOOR3) {
        printf("\nDerrote o boss antes de sair!\n");
        pauseGame();
    }
}

void movePlayer(char cmd) {
    int dx = 0, dy = 0;
    if (cmd == 'w') { dx = -1; p.dir = '^'; }
    else if (cmd == 's') { dx = 1; p.dir = 'v'; }
    else if (cmd == 'a') { dy = -1; p.dir = '<'; }
    else if (cmd == 'd') { dy = 1; p.dir = '>'; }

    int nx = p.x + dx, ny = p.y + dy;
    if (!isInside(nx, ny)) return;

    if (isEnemyAt(nx, ny) != -1 || lvl.map[nx][ny] == '#') {
        loseLife();
        return;
    }

    if (canMoveTo(nx, ny)) {
        p.x = nx;
        p.y = ny;
        if (lvl.map[p.x][p.y] == 'L') nextLevel();
        else if (lvl.map[p.x][p.y] == '=') { }
    }
}

int enemyCanMove(int x, int y) {
    if (!isInside(x, y)) return 0;
    char c = lvl.map[x][y];
    if (c == '*' || c == 'D' || c == 'k' || c == '@' || c == 'O' || c == 'L') return 0;
    if (isEnemyAt(x, y) != -1) return 0;
    return 1;
}

void moveMonsters() {
    int i;

    for (i = 0; i < lvl.monsterCount; i++) {
        if (!lvl.monsters[i].alive) continue;

        int dx = 0, dy = 0;
        char t = lvl.monsters[i].type;

        if (t == 'X') {
            int r = rand() % 4;
            if (r == 0) dx = -1;
            else if (r == 1) dx = 1;
            else if (r == 2) dy = -1;
            else dy = 1;
        } else if (t == 'Y') {
            int diffX = p.x - lvl.monsters[i].x;
            int diffY = p.y - lvl.monsters[i].y;
            if (abs(diffX) > abs(diffY)) dx = (diffX > 0) ? 1 : -1;
            else dy = (diffY > 0) ? 1 : -1;
        } else if (t == 'Z') {
            // Boss: alterna entre perseguir e criar espinho perto do jogador
            if (rand() % 3 == 0) {
                int sx = p.x + (rand() % 3 - 1);
                int sy = p.y + (rand() % 3 - 1);
                if (isInside(sx, sy) && lvl.map[sx][sy] == ' ') lvl.map[sx][sy] = '#';
            } else {
                int diffX = p.x - lvl.monsters[i].x;
                int diffY = p.y - lvl.monsters[i].y;
                if (abs(diffX) > abs(diffY)) dx = (diffX > 0) ? 1 : -1;
                else dy = (diffY > 0) ? 1 : -1;
            }
        }

        int nx = lvl.monsters[i].x + dx;
        int ny = lvl.monsters[i].y + dy;

        if (nx == p.x && ny == p.y) {
            loseLife();
            return;
        }
        if (enemyCanMove(nx, ny)) {
            lvl.monsters[i].x = nx;
            lvl.monsters[i].y = ny;
        }
    }
}

void tutorial() {
    clearScreen();
    printf("HISTORIA\n");
    printf("A vila foi tomada por monstros vindos de uma antiga masmorra.\n");
    printf("Escolha uma arma, atravesse tres andares e derrote o boss final.\n\n");

    printf("SIMBOLOS\n");
    printf("^ v < > jogador | * parede | # espinho | k caixa | O botao\n");
    printf("D porta fechada | @ chave | = porta aberta | L escada\n");
    printf("X monstro aleatorio | Y monstro perseguidor | Z boss final\n\n");

    printf("CONTROLES\n");
    printf("w/a/s/d: mover\n");
    printf("i: interagir\n");
    printf("o: atacar\n");
    printf("q: voltar ao menu\n");
    pauseGame();
}

void credits() {
    clearScreen();
    printf("Creditos\n");
    printf("Desenvolvedores: coloque aqui os nomes do grupo.\n");
    printf("Obrigado por jogar!\n");
}

void gameLoop() {
    char cmd;
    p.lives = 3;
    p.weapon = 0;
    loadLevel(VILLAGE);

    while (1) {
        drawMap();
        printf("Digite o comando: ");
        scanf(" %c", &cmd);
        getchar();

        if (cmd == 'q') break;
        else if (cmd == 'w' || cmd == 'a' || cmd == 's' || cmd == 'd') {
            movePlayer(cmd);
            moveMonsters();
        } else if (cmd == 'i') {
            interact();
            moveMonsters();
        } else if (cmd == 'o') {
            attack();
            moveMonsters();
        }
    }
}

void menu() {
    int op;
    while (running) {
        clearScreen();
        printf("========================\n");
        printf("     DUNGEON CRAWLER    \n");
        printf("========================\n");
        printf("1 - Jogar\n");
        printf("2 - Tutorial\n");
        printf("3 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &op);
        getchar();

        if (op == 1) gameLoop();
        else if (op == 2) tutorial();
        else if (op == 3) {
            credits();
            running = 0;
        }
    }
}

int main() {
    srand((unsigned)time(NULL));
    menu();
    return 0;
}
