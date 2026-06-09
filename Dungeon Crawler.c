#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifdef _WIN32
#define CLS "cls"
#else
#define CLS "clear"
#endif

#define MAX 25
#define VILA 0
#define A1 1
#define A2 2
#define A3 3

typedef struct{int x,y; char d; int v,ch,ar;} Jog;
typedef struct{int x,y,vivo; char t;} Mon;
typedef struct{
    char m[MAX][MAX];
    int l,c,andar,qtd,boss,botao;
    Mon mon[20];
} Fase;

Jog j;
Fase f;

void pause(){printf("\nPressione ENTER..."); getchar();}
void cls(){system(CLS);}
int dentro(int x,int y){return x>=0&&x<f.l&&y>=0&&y<f.c;}

void copia(const char *m[],int l,int c){
    int i,k;
    f.l=l; f.c=c;
    for(i=0;i<l;i++) for(k=0;k<c;k++) f.m[i][k]=m[i][k];
}

int monstro(int x,int y){
    int i;
    for(i=0;i<f.qtd;i++)
        if(f.mon[i].vivo&&f.mon[i].x==x&&f.mon[i].y==y) return i;
    return -1;
}

void achar(){
    int i,k;
    f.qtd=0; f.boss=6;
    for(i=0;i<f.l;i++) for(k=0;k<f.c;k++){
        if(strchr("^v<>",f.m[i][k])){
            j.x=i; j.y=k; j.d=f.m[i][k]; f.m[i][k]=' ';
        }
        if(strchr("XYZ",f.m[i][k])){
            f.mon[f.qtd].x=i;
            f.mon[f.qtd].y=k;
            f.mon[f.qtd].vivo=1;
            f.mon[f.qtd].t=f.m[i][k];
            f.qtd++;
            f.m[i][k]=' ';
        }
    }
}

void carregar(int a){
    f.andar=a; f.botao=0; j.ch=0;

    if(a==VILA){
        const char *m[]={
            "**********","*^   N   *","*        *","*   ***  *","*        *",
            "*        *","*  Bem   *","*        *","*      L *","**********"};
        copia(m,10,10);
    }else if(a==A1){
        const char *m[]={
            "**********","*^   @   *","* *** ** *","* k   D  *","* *** ** *",
            "*        *","*   kk   *","*        *","*      L *","**********"};
        copia(m,10,10);
    }else if(a==A2){
        const char *m[]={
            "***************","*^   @     X  *","* *** *****   *","*   #    k    *",
            "* *** *** *** *","*   O   D     *","* ***** *** ***","*     @       *",
            "*** ***** *** *","*      #      *","* *** *** *** *","*       D     *",
            "*   k      X  *","*          L  *","***************"};
        copia(m,15,15);
    }else{
        const char *m[]={
            "*************************","*^   @      k       Y   *","* ***** *********** *** *",
            "*     #      D          *","*** ***** ******* ***** *","*   O       @       #   *",
            "* ***** *** ***** ***** *","*     k       Y         *","* ********* ***** ***** *",
            "*     D       @         *","***** ***** ********* ***","*     #         k       *",
            "* *** ***** ***** ***** *","*   Y       D           *","* ***** ********* ***** *",
            "*       #       O       *","* ********* ***** ***** *","*       k         #     *",
            "***** ***** ********* ***","*           Y           *","* ***** *********** *****",
            "*         Z             *","* ***** *********** *** *","*                    L  *",
            "*************************"};
        copia(m,25,25);
    }
    achar();
}

void reiniciar(){
    int a=f.andar, ar=j.ar, v=j.v;
    carregar(a);
    j.ar=ar;
    j.v=v;
}

void dir(int *dx,int *dy){
    *dx=*dy=0;
    if(j.d=='^') *dx=-1;
    else if(j.d=='v') *dx=1;
    else if(j.d=='<') *dy=-1;
    else if(j.d=='>') *dy=1;
}

void desenhar(){
    int i,k,e;
    cls();
    printf("DUNGEON CRAWLER | Vidas:%d | Chaves:%d | Arma:",j.v,j.ch);
    if(j.ar==1) printf("Espada\n");
    else if(j.ar==2) printf("Arco\n");
    else if(j.ar==3) printf("Cajado\n");
    else printf("Nenhuma\n");

    if(f.andar==VILA) printf("Local: Vila\n\n");
    else printf("Andar: %d\n\n",f.andar);

    for(i=0;i<f.l;i++){
        for(k=0;k<f.c;k++){
            e=monstro(i,k);
            if(j.x==i&&j.y==k) printf("%c",j.d);
            else if(e!=-1) printf("%c",f.mon[e].t);
            else printf("%c",f.m[i][k]);
        }
        printf("\n");
    }
    printf("\nw/a/s/d mover | i interagir | o atacar | q voltar\n");
}

void perder(){
    j.v--;
    if(j.v<=0){
        cls();
        printf("====================\n      GAME OVER\n====================\n");
        pause();
        j.v=3; j.ar=0; carregar(VILA);
    }else{
        printf("\nVoce perdeu uma vida! A fase sera reiniciada.\n");
        pause();
        reiniciar();
    }
}

void arma(){
    int op;
    cls();
    printf("Escolha sua arma:\n1 - Espada\n2 - Arco\n3 - Cajado\nOpcao: ");
    scanf("%d",&op); getchar();
    if(op<1||op>3) op=1;
    j.ar=op;
    printf("\nArma escolhida!\n");
    pause();
}

void interagir(){
    int dx,dy,nx,ny;
    char c;
    dir(&dx,&dy);
    nx=j.x+dx; ny=j.y+dy;
    if(!dentro(nx,ny)) return;
    c=f.m[nx][ny];

    if(c=='N') arma();
    else if(c=='@'){
        j.ch++;
        f.m[nx][ny]=' ';
        printf("\nVoce pegou uma chave!\n");
        pause();
    }else if(c=='D'){
        if(j.ch>0){
            j.ch--;
            f.m[nx][ny]='=';
            printf("\nVoce abriu a porta!\n");
        }else printf("\nVoce precisa de uma chave.\n");
        pause();
    }else if(c=='O'){
        if(!f.botao){
            f.botao=1;
            printf("\nBotao ativado!\n");
            if(f.andar==A2) f.m[6][13]=' ';
            if(f.andar==A3) f.m[20][23]=' ';
        }else printf("\nBotao ja usado.\n");
        pause();
    }else{
        printf("\nNao ha nada para interagir.\n");
        pause();
    }
}

void dano(int x,int y){
    int e;
    if(!dentro(x,y)) return;
    e=monstro(x,y);

    if(e!=-1){
        if(f.mon[e].t=='Z'){
            f.boss--;
            printf("Voce acertou o boss! Vida: %d\n",f.boss);
            if(f.boss<=0){
                f.mon[e].vivo=0;
                cls();
                printf("============================\n          VITORIA!\n============================\n");
                printf("Voce derrotou o boss final.\n");
                pause();
                j.v=3; j.ar=0; carregar(VILA);
            }
        }else{
            f.mon[e].vivo=0;
            printf("Monstro derrotado!\n");
        }
    }

    if(f.m[x][y]=='k'){
        f.m[x][y]=' ';
        printf("Caixa destruida!\n");
    }
}

void atacar(){
    int dx,dy,i,k,x,y;
    if(j.ar==0){
        printf("\nVoce ainda nao escolheu arma.\n");
        pause();
        return;
    }

    dir(&dx,&dy);
    printf("\nAtaque realizado!\n");

    if(j.ar==1){
        for(i=1;i<=2;i++) for(k=-1;k<=1;k++){
            x=j.x+dx*i;
            y=j.y+dy*i;
            if(dx) y+=k;
            else x+=k;
            dano(x,y);
        }
    }else if(j.ar==2){
        for(i=1;i<=4;i++) dano(j.x+dx*i,j.y+dy*i);
    }else{
        for(i=-1;i<=1;i++) for(k=-1;k<=1;k++)
            if(i||k) dano(j.x+i,j.y+k);
    }
    pause();
}

int pode(int x,int y){
    char c;
    if(!dentro(x,y)) return 0;
    c=f.m[x][y];
    return !(c=='*'||c=='D'||c=='k'||c=='N'||c=='O');
}

void proximo(){
    if(f.andar==VILA) carregar(A1);
    else if(f.andar==A1) carregar(A2);
    else if(f.andar==A2) carregar(A3);
    else{
        printf("\nDerrote o boss antes de sair!\n");
        pause();
    }
}

void mover(char cmd){
    int dx=0,dy=0,nx,ny;

    if(cmd=='w'){dx=-1; j.d='^';}
    else if(cmd=='s'){dx=1; j.d='v';}
    else if(cmd=='a'){dy=-1; j.d='<';}
    else if(cmd=='d'){dy=1; j.d='>';}

    nx=j.x+dx; ny=j.y+dy;
    if(!dentro(nx,ny)) return;

    if(monstro(nx,ny)!=-1||f.m[nx][ny]=='#'){
        perder();
        return;
    }

    if(pode(nx,ny)){
        j.x=nx;
        j.y=ny;
        if(f.m[j.x][j.y]=='L') proximo();
    }
}

int podeMon(int x,int y){
    char c;
    if(!dentro(x,y)) return 0;
    c=f.m[x][y];
    return !(c=='*'||c=='D'||c=='k'||c=='@'||c=='O'||c=='L'||monstro(x,y)!=-1);
}

void moverMon(){
    int i,dx,dy,nx,ny,difx,dify,ex,ey,r;
    char t;

    for(i=0;i<f.qtd;i++){
        if(!f.mon[i].vivo) continue;

        dx=dy=0;
        t=f.mon[i].t;

        if(t=='X'){
            r=rand()%4;
            if(r==0) dx=-1;
            else if(r==1) dx=1;
            else if(r==2) dy=-1;
            else dy=1;
        }else if(t=='Y'){
            difx=j.x-f.mon[i].x;
            dify=j.y-f.mon[i].y;
            if(abs(difx)>abs(dify)) dx=difx>0?1:-1;
            else dy=dify>0?1:-1;
        }else if(t=='Z'){
            if(rand()%3==0){
                ex=j.x+(rand()%3-1);
                ey=j.y+(rand()%3-1);
                if(dentro(ex,ey)&&f.m[ex][ey]==' ') f.m[ex][ey]='#';
            }else{
                difx=j.x-f.mon[i].x;
                dify=j.y-f.mon[i].y;
                if(abs(difx)>abs(dify)) dx=difx>0?1:-1;
                else dy=dify>0?1:-1;
            }
        }

        nx=f.mon[i].x+dx;
        ny=f.mon[i].y+dy;

        if(nx==j.x&&ny==j.y){
            perder();
            return;
        }

        if(podeMon(nx,ny)){
            f.mon[i].x=nx;
            f.mon[i].y=ny;
        }
    }
}

void tutorial(){
    cls();
    printf("HISTORIA\nA vila foi tomada por monstros vindos da masmorra.\n");
    printf("Escolha uma arma, passe pelos andares e derrote o boss.\n\n");
    printf("SIMBOLOS\n^ v < > jogador | * parede | # espinho | k caixa | O botao\n");
    printf("D porta | @ chave | = porta aberta | L escada\n");
    printf("X aleatorio | Y perseguidor | Z boss\n\n");
    printf("COMANDOS\nw/a/s/d mover | i interagir | o atacar | q voltar\n");
    pause();
}

void creditos(){
    cls();
    printf("Creditos\nDesenvolvedores: Gabriel Monducci, Caio Gabriel e Alberto Neto.\nObrigado por jogar!\n");
    pause();
}

void jogo(){
    char cmd;
    j.v=3; j.ar=0;
    carregar(VILA);

    while(1){
        desenhar();
        printf("Digite o comando: ");
        scanf(" %c",&cmd);
        getchar();

        if(cmd=='q') break;
        if(cmd=='w'||cmd=='a'||cmd=='s'||cmd=='d'){
            mover(cmd);
            moverMon();
        }else if(cmd=='i'){
            interagir();
            moverMon();
        }else if(cmd=='o'){
            atacar();
            moverMon();
        }
    }
}

int main(){
    int op;
    srand(time(NULL));

    while(1){
        cls();
        printf("========================\n");
        printf("     DUNGEON CRAWLER\n");
        printf("========================\n");
        printf("1 - Jogar\n2 - Tutorial\n3 - Sair\nEscolha: ");
        scanf("%d",&op);
        getchar();

        if(op==1) jogo();
        else if(op==2) tutorial();
        else if(op==3){
            creditos();
            break;
        }
    }

    return 0;
}
