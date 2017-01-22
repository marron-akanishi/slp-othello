#include <stdio.h>
#include <stdbool.h>

#define SIZE 8

typedef enum {WHT=-1, EMP=0, BLK=+1, OUT=+2} Piece;

typedef struct {
    Piece cell[SIZE][SIZE];
} Board;

typedef struct{
    int x;
    int y;
    Piece color;
} Hand;

Board brd_init(void);	// 盤面の初期化
void brd_output(Board brd);	// 盤面の出力
void player(Board *brd, Piece wh, bool *pass);	// プレイヤの処理
void hand_input(Hand *hand);	// 打ち手の入力
bool brd_turn(Board *brd, Hand hand, bool check);	// 盤面の更新
int brd_turn_dir(Board *brd, Hand hand, int dx, int dy, bool check);	// 指定方向への走査
bool pass_check(Board *brd, Piece wh);	// パスの判定
bool end_check(Board brd);	// 終了の判定
Piece winner_check(Board brd);	// 勝敗の判定
Piece cell_val(Board brd, int x, int y);	// 指定位置のマスの状態

int main(void){
    //var
    Board brd;
    bool passBLK, passWHT;

    brd = brd_init();
    brd_output(brd);

    while(!end_check(brd)){
        printf("Ｘの番です。\n");
        player(&brd,BLK,&passBLK); // 先手(黒)の処理
        if(!passBLK) brd_output(brd);
        else printf("置ける場所がありません。\n");
        printf("Ｏの番です。\n");
        player(&brd,WHT,&passWHT);	// 後手(白)の処理
        if(!passWHT) brd_output(brd);
        else printf("置ける場所がありません。\n");
        if ( passBLK && passWHT ) break;	// パスによる終了の判定
    }

    //-- 勝敗判定
    ( winner_check(brd) == BLK ) ? printf("Ｘの勝ち\n") :  printf("Ｏの勝ち\n");

    return 0;
}

Board brd_init(void) {
    //-- 宣言
    Board tmp = {{
        {EMP, EMP, EMP, EMP, EMP, EMP, EMP, EMP},
        {EMP, EMP, EMP, EMP, EMP, EMP, EMP, EMP},
        {EMP, EMP, EMP, EMP, EMP, EMP, EMP, EMP},
        {EMP, EMP, EMP, WHT, BLK, EMP, EMP, EMP},
        {EMP, EMP, EMP, BLK, WHT, EMP, EMP, EMP},
        {EMP, EMP, EMP, EMP, EMP, EMP, EMP, EMP},
        {EMP, EMP, EMP, EMP, EMP, EMP, EMP, EMP},
        {EMP, EMP, EMP, EMP, EMP, EMP, EMP, EMP}
    }};

    return tmp;
}

void brd_output(Board brd) {
    //-- 宣言
    int x, y;    // 反復変数

    //-- 出力処理
    printf("  ");
    for ( y = 0; y < SIZE; y++ ) { printf("Y%d",y);}
    for ( x = 0; x < SIZE; x++) {
        printf("\nX%d",x);
        for ( y = 0; y < SIZE; y++ ) {
            switch ( brd.cell[x][y] ) {
                case WHT: printf("Ｏ");   break;
                case EMP: printf("・");   break;
                case BLK: printf("Ｘ");   break;
                default : printf("？");
            }
        }
    }
    printf("\n\n");
}

void player(Board *brd, Piece wh, bool *pass) {
    //-- 宣言
    Hand hand;	// 打ち手格納用変数
    hand.color = wh;
    *pass = false;

    if(pass_check(brd,wh)){
        *pass = true;
        return;
    }
    while ( 1 ) {
        //-- 打ち手入力
        hand_input(&hand);
        //-- 盤面更新
        if(brd_turn(brd,hand,false)) break;
        else printf("置けませんでした...\n");
    }
    printf("%d %d\n", hand.x, hand.y);
}

void hand_input(Hand *hand) {
    //-- 宣言
    int x, y;	// 入力格納用変数

    //--入力処理
    while ( 1 ) {
        printf("X > ");
        scanf("%d", &x);
        if ( x >= 0 && x < SIZE ) { break; }	// 正しい入力なら反復を脱出
        printf("不正な入力です。  再入力してください。\n");
    }

    while ( 1 ) {
        printf("Y > ");
        scanf("%d", &y);
        if ( y >= 0 && y < SIZE ) { break; }	// 正しい入力なら反復を脱出
        printf("不正な入力です。  再入力してください。\n");
    }

    //-- 格納
    hand->x = x;
    hand->y = y;
}

bool brd_turn(Board *brd, Hand hand, bool check) {
    //-- 宣言
    int count = 0;	// ひっくり返した駒の数

    //-- 打つ位置がEMPでないなら失敗
    if ( cell_val(*brd, hand.x, hand.y) != EMP ) return false;

    //-- 打つ位置から上下左右斜め８方向への走査
    for(int x = -1;x < 2;x++){
        for(int y = -1;y < 2;y++){
            if(x == 0 && y == 0) continue;
            count += brd_turn_dir(brd, hand, x, y, check);
        }
    }

    //-- 駒をひっくり返せなければ失敗
    if ( count == 0 ) return false;

    //-- 返却
    return true;
}

int brd_turn_dir(Board *brd, Hand hand, int dx, int dy, bool check) {
    //-- 宣言
    int tx = hand.x;	// 現在位置
    int ty = hand.y;	// 現在位置
    Piece wh = hand.color;	// 自分の駒
    int len = 0;	// 相手の駒の連長

    //-- はさんでいるか判定
    while ( 1 ) {
        //- 現在位置を指定方向に更新
        tx += dx;
        ty += dy;

        //ひとつ先が自分かどうかを見る
        if(cell_val(*brd,tx+dx,ty+dy) == wh) return 0; 

        //- 駒でないなら0を返す、相手なら連長追加
        if(cell_val(*brd,tx,ty) == OUT || cell_val(*brd,tx,ty) == EMP) return 0;
        else if(cell_val(*brd,tx,ty) != wh) len++;
        //- 連長が正で自分の駒なら打ち切り
        if(len > 0 && cell_val(*brd,tx,ty) == wh) break;
    }

    //-- 駒をひっくり返す
    while ( 1 ) {
        //- 現在位置から指定方向の逆へ更新
        tx -= dx;
        ty -= dy;
        //- 駒が相手の駒でないなら打ち切り
        if(cell_val(*brd,tx,ty) == OUT || cell_val(*brd,tx,ty) == EMP || cell_val(*brd,tx,ty) == wh) break;
        //- 自分の駒に更新
        if(!check)brd->cell[tx][ty] = wh;
    }

    //-- 打つ位置に自分の駒を置く
    if(!check)brd->cell[hand.x][hand.y] = wh;

    //-- 返却
    return len;
}

bool pass_check(Board *brd, Piece wh) {
    int x, y;	// 反復変数
    Hand tmp;
    tmp.color = wh;

    //-- おける場所があるか走査
    for ( x = 0; x < SIZE; x++ ) {
        tmp.x = x;
        for ( y = 0; y < SIZE; y++ ) {
            tmp.y = y;
            if ( brd_turn(brd, tmp, true) ) return false;
        }
    }

    //-- 返却
    return true;
}

bool end_check(Board brd) {
    int x, y;
    for ( x = 0; x < SIZE; x++ ) {
        for ( y = 0; y < SIZE; y++ ) {
            if ( brd.cell[x][y] == EMP ) {   // brd.cell[x][y]がEMPか調べる
                return false;                // EMPならfalseを返す
            }
        }
    }
    return true;    // EMPがなければ終了
}

Piece winner_check(Board brd) {
    //-- 宣言
    int x, y;	// 反復変数
    int black, white;	// 駒の数
    black = white = 0;

    //-- カウント
    for ( x = 0; x < SIZE; x++ ) {
        for ( y = 0; y < SIZE; y++ ) {
            if(brd.cell[x][y] == BLK) black++;
            else if(brd.cell[x][y] == WHT) white++;
        }
    }

    //-- 多いほうを返却
    if(white > black) return WHT;
    else return BLK;
}

Piece cell_val(Board brd, int x, int y) {
    //-- 盤外ならOUTを返却
    if ( x < 0 || x >= SIZE ) { return OUT; }
    if ( y < 0 || y >= SIZE ) { return OUT; }

    //-- 駒の状態を返却
    return brd.cell[x][y];
}