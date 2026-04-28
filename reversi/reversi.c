#include <stdio.h>
#include <synchapi.h>/*Sleep()を使うためにinclude*/

/*引数のx, yにplayerDiskのコマを置けるかどうかをチェックし、fCheckOnlyなら1枚でも置ける状況であれば盤面は変えずにreturn 1
そうでないならひっくり返せるものをすべてひっくり返してひっくり返した枚数をreturn。
1枚でもひっくり返せている場合、mass[y][x]に自分のコマを置くが1枚もひっくり返せなかった場合は置かない*/
int PlaceDisk(char (*mass)[8], int getX, int getY, char disk[2], int playerIndex, char emptyMass, int fCheckOnly)
{
    /*左上*/
    int x, y;
    int nTurnDisk = 0;
    if((0 < getX && 0 < getY) && (mass[getY-1][getX-1] == disk[(playerIndex+1)%2])){/*すぐ左上が敵のコマじゃないとそもそもひっくり返せない*/
        for(x = getX-2, y = getY-2; 0 <= x && 0 <= y; x--, y--){
            if(mass[y][x] == disk[playerIndex]){/*敵のコマを挟んで自分のコマがあった*/
                if(fCheckOnly){return 1;}/*チェックだけならreturn*/
                for(x++, y++; x < getX && y < getY; x++, y++){/*見つけた自分のコマと今置いたコマの間のコマを全てひっくりかえす*/
                    mass[y][x] = disk[playerIndex];
                    nTurnDisk++;
                }
                break;
            }else if(mass[y][x] == emptyMass){/*敵のコマの向こうに何もなかった(自分のコマがあったとしてもそれより先に何も置かれていないマスがあった)*/
                break;/*何もひっくり返さない*/
            }
        }
    }

    /*右上*/
    if((getX < 7 && 0 < getY) && (mass[getY-1][getX+1] == disk[(playerIndex+1)%2])){/*すぐ右上が敵のコマじゃないとそもそもひっくり返せない*/
        for(x = getX+2, y = getY-2; x < 8 && 0 <= y; x++, y--){
            if(mass[y][x] == disk[playerIndex]){/*敵のコマを挟んで自分のコマがあった*/
                if(fCheckOnly){return 1;}/*チェックだけならreturn*/
                for(x--, y++; getX < x && y < getY; x--, y++){/*見つけた自分のコマと今置いたコマの間のコマを全てひっくりかえす*/
                    mass[y][x] = disk[playerIndex];
                    nTurnDisk++;
                }
                break;
            }else if(mass[y][x] == emptyMass){/*敵のコマの向こうに何もなかった(自分のコマがあったとしてもそれより先に何も置かれていないマスがあった)*/
                break;/*何もひっくり返さない*/
            }
        }
    }

    /*右下*/
    if((getX < 7 && getY < 7) && (mass[getY+1][getX+1] == disk[(playerIndex+1)%2])){/*すぐ右下が敵のコマじゃないとそもそもひっくり返せない*/
        for(x = getX+2, y = getY+2; x < 8 && y < 8; x++, y++){
            if(mass[y][x] == disk[playerIndex]){/*敵のコマを挟んで自分のコマがあった*/
                if(fCheckOnly){return 1;}/*チェックだけならreturn*/
                for(x--, y--; getX < x && getY < y; x--, y--){/*見つけた自分のコマと今置いたコマの間のコマを全てひっくりかえす*/
                    mass[y][x] = disk[playerIndex];
                    nTurnDisk++;
                }
                break;
            }else if(mass[y][x] == emptyMass){/*敵のコマの向こうに何もなかった(自分のコマがあったとしてもそれより先に何も置かれていないマスがあった)*/
                break;/*何もひっくり返さない*/
            }
        }
    }

    /*左下*/
    if((0 < getX && getY < 7) && (mass[getY+1][getX-1] == disk[(playerIndex+1)%2])){/*すぐ左下が敵のコマじゃないとそもそもひっくり返せない*/
        for(x = getX-2, y = getY+2; 0 <= x && y < 8; x--, y++){
            if(mass[y][x] == disk[playerIndex]){/*敵のコマを挟んで自分のコマがあった*/
                if(fCheckOnly){return 1;}/*チェックだけならreturn*/
                for(x++, y--; x < getX && getY < y; x++, y--){/*見つけた自分のコマと今置いたコマの間のコマを全てひっくりかえす*/
                    mass[y][x] = disk[playerIndex];
                    nTurnDisk++;
                }
                break;
            }else if(mass[y][x] == emptyMass){/*敵のコマの向こうに何もなかった(自分のコマがあったとしてもそれより先に何も置かれていないマスがあった)*/
                break;/*何もひっくり返さない*/
            }
        }
    }

    /*上*/
    if(0 < getY && mass[getY-1][getX] == disk[(playerIndex+1)%2]){/*すぐ上が敵のコマじゃないとそもそもひっくり返せない*/
        for(x = getX, y = getY-2; 0 <= y; y--){
            if(mass[y][x] == disk[playerIndex]){/*敵のコマを挟んで自分のコマがあった*/
                if(fCheckOnly){return 1;}/*チェックだけならreturn*/
                for(y++; y < getY; y++){/*見つけた自分のコマと今置いたコマの間のコマを全てひっくりかえす*/
                    mass[y][x] = disk[playerIndex];
                    nTurnDisk++;
                }
                break;
            }else if(mass[y][x] == emptyMass){/*敵のコマの向こうに何もなかった(自分のコマがあったとしてもそれより先に何も置かれていないマスがあった)*/
                break;/*何もひっくり返さない*/
            }
        }
    }

    /*下*/
    if(getY < 7 && mass[getY+1][getX] == disk[(playerIndex+1)%2]){/*すぐ上が敵のコマじゃないとそもそもひっくり返せない*/
        for(x = getX, y = getY+2; y < 8; y++){
            if(mass[y][x] == disk[playerIndex]){/*敵のコマを挟んで自分のコマがあった*/
                if(fCheckOnly){return 1;}/*チェックだけならreturn*/
                for(y--; getY < y; y--){/*見つけた自分のコマと今置いたコマの間のコマを全てひっくりかえす*/
                    mass[y][x] = disk[playerIndex];
                    nTurnDisk++;
                }
                break;
            }else if(mass[y][x] == emptyMass){/*敵のコマの向こうに何もなかった(自分のコマがあったとしてもそれより先に何も置かれていないマスがあった)*/
                break;/*何もひっくり返さない*/
            }
        }
    }

    /*左*/
    if(0 < getX && mass[getY][getX-1] == disk[(playerIndex+1)%2]){/*すぐ上が敵のコマじゃないとそもそもひっくり返せない*/
        for(x = getX-2, y = getY; 0 <= x; x--){
            if(mass[y][x] == disk[playerIndex]){/*敵のコマを挟んで自分のコマがあった*/
                if(fCheckOnly){return 1;}/*チェックだけならreturn*/
                for(x++; x < getX; x++){/*見つけた自分のコマと今置いたコマの間のコマを全てひっくりかえす*/
                    mass[y][x] = disk[playerIndex];
                    nTurnDisk++;
                }
                break;
            }else if(mass[y][x] == emptyMass){/*敵のコマの向こうに何もなかった(自分のコマがあったとしてもそれより先に何も置かれていないマスがあった)*/
                break;/*何もひっくり返さない*/
            }
        }
    }

    /*右*/
    if(getX < 7 && mass[getY][getX+1] == disk[(playerIndex+1)%2]){/*すぐ上が敵のコマじゃないとそもそもひっくり返せない*/
        for(x = getX+2, y = getY; x < 8; x++){
            if(mass[y][x] == disk[playerIndex]){/*敵のコマを挟んで自分のコマがあった*/
                if(fCheckOnly){return 1;}/*チェックだけならreturn*/
                for(x--; getX < x; x--){/*見つけた自分のコマと今置いたコマの間のコマを全てひっくりかえす*/
                    mass[y][x] = disk[playerIndex];
                    nTurnDisk++;
                }
                break;
            }else if(mass[y][x] == emptyMass){/*敵のコマの向こうに何もなかった(自分のコマがあったとしてもそれより先に何も置かれていないマスがあった)*/
                break;/*何もひっくり返さない*/
            }
        }
    }

    if(nTurnDisk){
        /*1枚以上ひっくり返せたので自分のコマを置く*/
        mass[getY][getX] = disk[playerIndex];
    }

    return nTurnDisk;
}

/*最初と最後に改行が入ります*/
void ShowMoveText(char *strShowText, int nSideSpace, int nTimeMax/*ミリ秒*/, int nTimeEndStop)
{
    /*1秒かけて(分解能を50ミリ秒として20回に分けて)上に凸の2次関数[x: 0.0 ~ 1.0]で移動*/
    int nSleepLength = 20;
    int nShowTextLength = strlen(strShowText);
    int nTotalTextLength = nShowTextLength + nSideSpace;
    printf("\n");
    for(int i = 0, iMax = nTimeMax / nSleepLength; i < iMax; i++){
        printf("\r");
        float flPercent = 1.0f - (float)i / (float)iMax;
        float flPercent2 = 1.0f - flPercent * flPercent;
        int nVisibleLength = (int)(flPercent2 * (float)nTotalTextLength);
        if(nVisibleLength > nShowTextLength){
            for(int j = 0; j < nVisibleLength - nShowTextLength; j++){
                printf(" ");
            }
            nVisibleLength = nShowTextLength;
        }
        printf("%s", &strShowText[nShowTextLength - nVisibleLength]);
        Sleep(nSleepLength);
    }

    /*最後に(念のため)普通に描画*/
    printf("\r");
    for(int i = 0; i < nSideSpace; i++){
        printf(" ");
    }
    printf("%s\n", strShowText);

    Sleep(nTimeEndStop);

    return;
}

void ShowResultBoard(int boardWidth, int boardHeight, int *nDiskCount, char *disk, int nPlayer, char emptyMass)
{
    int playerIndex = 0;
    int nShowDisk = 0;
    int fSkip = 0;/*盤面が埋まる前に試合が終了した場合、空のマスを表示するがそれにいちいちSleep()はいらない*/

    printf(" +");
    for(int x = 0; x < 8; x++){
        printf("  ");
    }
    printf(" +\n");
    for(int y = 0; y < boardHeight; y++){
        printf("  ");
        for(int x = 0; x < boardWidth; x++){
            while(playerIndex < nPlayer && nDiskCount[playerIndex] <= nShowDisk){
                playerIndex++;
                nShowDisk = 0;
            }
            if(playerIndex < nPlayer){
                printf(" %c", disk[playerIndex]);
                nShowDisk++;
            }else{
                fSkip = 1;
                printf(" %c", emptyMass);
            }
            if(fSkip){
                Sleep(10);
            }else{
                Sleep(50);
            }
        }
        printf("  \n");
    }
    printf(" +");
    for(int x = 0; x < 8; x++){
        printf("  ");
    }
    printf(" +\n");

    return;
}

int main(void)
{
    /*' 'ならなにもなし、コマは'X','O'*/
    char mass[8][8];
    
    /*初期化*/
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            mass[i][j] = ' ';
        }
    }
    mass[3][4] = 'O';
    mass[4][3] = 'O';
    mass[3][3] = 'X';
    mass[4][4] = 'X';

//     char mass[8][8] = {
// {' ', 'X', 'X', 'X', 'X', 'X', ' ', ' ',},
// {'X', 'O', 'X', 'X', 'X', 'X', ' ', ' ',},
// {'X', 'X', 'X', 'O', 'O', 'X', 'X', 'O',},
// {'X', 'X', 'X', 'O', 'X', 'X', 'X', 'O',},
// {'X', 'X', 'X', 'X', 'O', 'X', 'X', 'O',},
// {' ', ' ', 'X', 'X', 'X', 'O', 'O', 'O',},
// {' ', ' ', 'X', ' ', ' ', 'O', 'X', 'O',},
// {' ', ' ', 'X', ' ', 'O', 'O', 'O', 'O',},};
    
    ShowMoveText("リバーシ!!!", 8, 800, 400);
    printf("\n");
    printf("コマは X と O です。X から始まります。\n座標は左上が0, 0、右上が7, 0、右下が7, 7です\n");

    char disk[2] = {'X', 'O'};
    for(int iTurn = 0/*偶数なら黒の手番*/, fSkipBefore = 0/*2連続でパスが発生した場合、試合終了になる*/; 1; iTurn++){
        /*盤面を表示*/
        printf(" +");
        for(int x = 0; x < 8; x++){
            printf(" %d", x);
        }
        printf(" +\n");
        for(int y = 0; y < 8; y++){
            printf(" %d", y);
            for(int x = 0; x < 8; x++){
                printf(" %c", mass[y][x]);
            }
            printf(" %d\n", y);
        }
        printf(" +");
        for(int x = 0; x < 8; x++){
            printf(" %d", x);
        }
        printf(" +\n");

        /*手番を表示*/
        printf("%c の手番\n", disk[iTurn % 2]);

        /*置ける場所がない場合、パスとなるのでパスではないかを調べる*/
        int fSkip = 1;
        int nEmptyMass = 0;
        int nDiskCount = 0;/*手番のプレイヤーの枚数*/
        for(int y = 0; y < 8; y++){
            for(int x = 0; x < 8; x++){
                for(int n = 0; n < 2; n++){
                    if(mass[y][x] == disk[iTurn % 2]){
                        nDiskCount++;
                    }
                }
                if(mass[y][x] == ' '){
                    nEmptyMass++;
                    if(PlaceDisk(mass, x, y, disk, iTurn % 2, ' ', 1)){/*1か所でも置ける場所があればユーザーの入力を待機する(チェック終了)*/
                        fSkip = 0;
                        x = y = 8;
                    }
                }
            }
        }

        /*置ける場所がなかったのでスキップ*/
        if(fSkip){
            if(!nEmptyMass){
                /*全てのマスにコマが置かれているため試合を終了する*/
                printf("盤面が全て埋まったため試合終了\n");
                break;
            }
            if(!nDiskCount){
                /*自分のコマがなくもう負けが決まっているため試合を終了する*/
                printf("決着がついたため試合終了\n");
                break;
            }
            printf("置ける場所がないためパス\n");
            if(fSkipBefore){
                printf("両者パスのため試合終了\n");
                break;/*2連続でパスが発生した場合、試合終了になる*/
            }else{
                fSkipBefore = 1;
            }
            continue;
        }else{
            fSkipBefore = 0;
        }

        /*置く場所を問い合わせ*/
        int getX, getY;
        int nTurnDisk = 0;/*めくれた枚数。1枚もめくれないところには置けない*/
        do{
            printf("どこに置きますか？ >> ");
            scanf("%d%d", &getX, &getY);
            if(getX == -99 && getY == -99){
                printf("強制終了しますか？(y/n) >> ");
                char answer;
                scanf("%c", &answer);
                scanf("%c", &answer);/*直前のEnterを食べる*/
                if(answer == 'y'){
                    printf("終了します");
                    return 0;
                }
            }else if(!(0 <= getX && getX < 8 && 0 <= getY && getY < 8)){
                /*indexが範囲外*/
                printf("0 ~ 7 の範囲で座標を指定してください\n");
            }else if(mass[getY][getX] == ' '){
                /*そのマスに何もない場合、置いて1枚でもめくれるかどうかをチェック*/
                if(PlaceDisk(mass, getX, getY, disk, iTurn % 2, ' ', 0)){
                    /*置けた(ひっくり返せた)のでOK*/
                    break;
                }else{
                    printf("1枚もめくれない場所には置けません。\n");
                }
            }else{
                /*既にそこにコマがある場合、聞き直す*/
                printf("そこにはすでに %c のコマがあります\n", mass[getY][getX]);
            }
        }while(1);

        printf("\n");
    }

    /*集計*/
    int nDiskCount[2] = {0, 0};
    for(int y = 0; y < 8; y++){
        for(int x = 0; x < 8; x++){
            for(int n = 0; n < 2; n++){
                if(mass[y][x] == disk[n]){
                    nDiskCount[n]++;
                }
            }
        }
    }

    /*リザルトを盤面で表示*/
    ShowResultBoard(8, 8, nDiskCount, disk, 2, ' ');
    
    /*リザルトを表示*/
    printf("\n");
    ShowMoveText("--結果--", 8, 800, 400);
    printf("\n");
    for(int n = 0; n < 2; n++){
        printf("%c : %d 枚\n", disk[n], nDiskCount[n]);
    }
    if(nDiskCount[0] == nDiskCount[1]){
        printf("引き分け");
    }else if(nDiskCount[0] > nDiskCount[1]){
        printf("%c の勝ち", disk[0]);
    }else if(nDiskCount[0] < nDiskCount[1]){
        printf("%c の勝ち", disk[1]);
    }else{
        printf("エラー");
    }
    printf("\n");

    return 0;
}


