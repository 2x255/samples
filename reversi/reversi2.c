#include <stdio.h>
#include <synchapi.h>/*Sleep()を使うためにinclude*/
#include <stdlib.h>

/*引数のx, yに対してxDir, yDirの方向(-1,0,1のいずれかを指定)playerDiskのコマを置けるかどうかをチェックし、fCheckOnlyなら1枚でも置ける状況であれば盤面は変えずにreturn 1
そうでないならひっくり返せるものをすべてひっくり返してひっくり返した枚数をreturn。
1枚でもひっくり返せている場合、mass[y][x]に自分のコマを置くが1枚もひっくり返せなかった場合は置かない*/
int PlaceDiskDirection(char **mass, int getX, int getY, int xDir, int yDir, int xMax, int yMax, char disk[2], int playerIndex, char emptyMass, int fCheckOnly)
{
    int x, y;
    int nTurnDisk = 0;
    if((0 <= getX+xDir && getX+xDir < xMax) && (0 <= getY+yDir && getY+yDir < yMax) && (mass[getY+yDir][getX+xDir] == disk[(playerIndex+1)%2])){/*すぐ1つ隣が敵のコマじゃないとそもそもひっくり返せない*/
        for(x = getX+xDir*2, y = getY+yDir*2; (0 <= x && x < xMax) && (0 <= y && y < yMax); x+=xDir, y+=yDir){
            if(mass[y][x] == disk[(playerIndex+1)%2]){/*敵のコマが続く限り判定は継続*/
            }else if(mass[y][x] == disk[playerIndex]){/*敵のコマを挟んで自分のコマがあった*/
                if(fCheckOnly){return 1;}/*チェックだけならreturn*/
                for(x-=xDir, y-=yDir; x != getX || y != getY; x-=xDir, y-=yDir){/*見つけた自分のコマと今置いたコマの間のコマを全てひっくりかえす*/
                    mass[y][x] = disk[playerIndex];
                    nTurnDisk++;
                }
                break;
            }else if(mass[y][x] == emptyMass){/*敵のコマの向こうに何もなかった(自分のコマがあったとしてもそれより先に何も置かれていないマスがあった)*/
                break;/*何もひっくり返さない*/
            }else{
                printf("エラー: %d\nxDir: %d, yDir: %d, x: %d, y: %d", __LINE__, xDir, yDir, x, y);
                return 0;
            }
        }
    }

    return nTurnDisk;
}
int PlaceDisk(char **mass, int getX, int getY, int xMax, int yMax, char disk[2], int playerIndex, char emptyMass, int fCheckOnly)
{
    int nTurnDisk = 0;

    for(int yDir = -1; yDir <= 1; yDir++){
        for(int xDir = -1; xDir <= 1; xDir++){
            nTurnDisk += PlaceDiskDirection(mass, getX, getY, xDir, yDir, xMax, yMax, disk, playerIndex, emptyMass, fCheckOnly);
            if(fCheckOnly && nTurnDisk){
                return 1;
            }
        }
    }

    if(nTurnDisk){
        /*1枚以上ひっくり返せたので自分のコマを置く*/
        mass[getY][getX] = disk[playerIndex];
    }

    return nTurnDisk;
}

/*左から減速で出てくるテキストを表示。最初と最後に改行が入ります*/
void ShowMoveText(char *strShowText, int nSideSpace, int nTimeMax/*ミリ秒*/, int nTimeEndStop)
{
    /*1秒かけて(分解能\を50ミリ秒として20回に分けて)上に凸の2次関数[x: 0.0 ~ 1.0]で移動*/
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

/*結果を盤面に順にコマが並ぶアニメーションで表\示*/
void ShowResultBoard(int boardWidth, int boardHeight, int *nDiskCount, char *disk, int nPlayer, int nTimeMax/*ミリ秒*/, char emptyMass)
{
    int playerIndex = 0;
    int nShowDisk = 0;
    int fSkip = 0;/*盤面が埋まる前に試合が終了した場合、空のマスを表\示するがそれにいちいちSleep()はいらない*/
    int nDiskTotal = 0;

    for(int i = 0; i < nPlayer; i++){
        nDiskTotal += nDiskCount[i];
    }
    
    printf(" +");
    for(int x = 0; x < boardWidth; x++){
        printf("  ");
    }
    printf(" +\n");
    for(int y = 0, nSleepTime = (int)((float)nTimeMax / (float)(nDiskTotal + (boardWidth * boardHeight - nDiskTotal) / 5)); y < boardHeight; y++){
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
                Sleep(nSleepTime / 5 + 1);
            }else{
                Sleep(nSleepTime);
            }
        }
        printf("  \n");
    }
    printf(" +");
    for(int x = 0; x < boardWidth; x++){
        printf("  ");
    }
    printf(" +\n");

    return;
}

/*盤面を表\示。最後に改行を入れます*/
void ShowBoard(char **mass, int boardWidth, int boardHeight)
{
    printf(" +");
    for(int x = 0; x < boardWidth; x++){
        printf(" %d", x%10);
    }
    printf(" +\n");
    for(int y = 0; y < boardHeight; y++){
        printf(" %d", y%10);
        for(int x = 0; x < boardWidth; x++){
            printf(" %c", mass[y][x]);
        }
        printf(" %d\n", y%10);
    }
    printf(" +");
    for(int x = 0; x < boardWidth; x++){
        printf(" %d", x%10);
    }
    printf(" +\n");

    return;
}

int main(int argc, char *argv[])
{
    /*オプションを認識、初期値を決定*/
    int boardWidth = 8, boardHeight = 8;
    char emptyMass = ' ';
    char fAuto[2] = {0, 0};/*playerIndexをindexに使う*/
    char disk[2] = {'X', 'O'};
    char fBoardHidden = 0;
    int nResultShowTime = 2000;/*ミリ秒*/
    int argvIndex = 1;/*0はソ\フト名*/
    if(2 <= argc){
        char fEnabled;
        do{
            fEnabled = 0;
            /*width, height*/
            if(3 <= (argc - argvIndex) && !strcmp(argv[argvIndex], "--size")){
                argvIndex++;
                printf("カスタムサイズ\n");
                boardWidth = atoi(argv[argvIndex++]);
                boardHeight = atoi(argv[argvIndex++]);
                if(boardWidth < 2 || boardHeight < 2){
                    printf("パラメータが無効です: %d\n", __LINE__);
                    return 0;
                }
                fEnabled = 1;
            }
            /*先手CPU化フラグ, 後手CPU化フラグ, シード値*/
            if(4 <= (argc - argvIndex) && !strcmp(argv[argvIndex], "--auto")){
                argvIndex++;
                for(int i = 0; i < 2; i++){
                    fAuto[i] = atoi(argv[argvIndex++]);
                }
                printf("自動対戦モード\n");
                srand(atoi(argv[argvIndex++]));
                fEnabled = 1;
            }
            /*-*/
            if(1 <= (argc - argvIndex) && !strcmp(argv[argvIndex], "--hidden")){
                argvIndex++;
                fBoardHidden = 1;
                printf("盤面なしモード\n");
                fEnabled = 1;
            }
            /*リザルト表\示時間(ミリ秒)*/
            if(2 <= (argc - argvIndex) && !strcmp(argv[argvIndex], "--result")){
                argvIndex++;
                nResultShowTime = atoi(argv[argvIndex++]);
                printf("結果表\示時間を指定: %dミリ秒\n", nResultShowTime);
                fEnabled = 1;
            }
        }while(fEnabled);
    }

    /*初期化*/
    char **mass = NULL;
    if(!(mass = malloc(sizeof(char *) * boardHeight))){
        printf("メモリ確保失敗: %d\n", __LINE__);
        return 0;
    }
    for(int y = 0; y < boardHeight; y++){
        if(!(mass[y] = malloc(sizeof(char) * boardWidth))){
            printf("メモリ確保失敗: %d\n", __LINE__);
            return 0;
        }
        for(int x = 0; x < boardWidth; x++){
            mass[y][x] = emptyMass;
        }
    }
    mass[boardHeight/2-1][boardWidth/2] = 'O';
    mass[boardHeight/2][boardWidth/2-1] = 'O';
    mass[boardHeight/2-1][boardWidth/2-1] = 'X';
    mass[boardHeight/2][boardWidth/2] = 'X';

    ShowMoveText("リバーシ!!!", boardWidth, 800, 400);
    printf("\n");
    printf("コマは X と O です。X から始まります。\n座標は左上が0, 0、右上が%d, 0、右下が%d, %dです\n", boardWidth-1, boardWidth-1, boardHeight-1);
    
    /*もっとも端にあるコマのindexを保存。乱数生成の時間短縮に使う*/
    int leftSideDiskPos = boardWidth/2-1, rightSideDiskPos = boardWidth/2, topSideDiskPos = boardHeight/2-1, bottomSideDiskPos = boardHeight/2;

    for(int iTurn = 0/*偶数なら黒の手番*/, fSkipBefore = 0/*2連続でパスが発生した場合、試合終了になる*/; 1; iTurn++){
        /*盤面を表\示*/
        if(!fBoardHidden){
            ShowBoard(mass, boardWidth, boardHeight);
        }

        /*手番を表\示*/
        printf("%c の手番\n", disk[iTurn % 2]);

        /*置ける場所がない場合、パスとなるのでパスではないかを調べる*/
        int fSkip = 1;
        int nEmptyMass = 0;
        int nDiskCount = 0;/*手番のプレイヤーの枚数*/
        for(int y = 0; y < boardHeight; y++){
            for(int x = 0; x < boardWidth; x++){
                for(int n = 0; n < 2; n++){
                    if(mass[y][x] == disk[iTurn % 2]){
                        nDiskCount++;
                    }
                }
                if(mass[y][x] == emptyMass){
                    nEmptyMass++;
                    if(PlaceDisk(mass, x, y, boardWidth, boardHeight, disk, iTurn % 2, emptyMass, 1)){/*1か所でも置ける場所があればユーザーの入力を待機する(チェック終了)*/
                        fSkip = 0;
                        /*x, yの両方のループから出る*/
                        x = boardWidth;
                        y = boardHeight;
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
        char fShowRequest = 1;/*入力試行中の要求文表\示*/
        if(fAuto[iTurn%2]){
            fShowRequest = 0;
            printf("どこに置きますか？ >> ");
        }
        do{
            if(fShowRequest){
                printf("どこに置きますか？ >> ");
            }
            if(fAuto[iTurn%2]){/*機械乱数*/                                   /*例えば、この↓この+1がなくて幅の差が1のとき、%1は常に0になってしまうので+1*/
                getX = ((leftSideDiskPos) + rand() % (rightSideDiskPos - leftSideDiskPos + 1)) + (rand() % 3 - 1);
                getY = ((topSideDiskPos) + rand() % (bottomSideDiskPos - topSideDiskPos + 1)) + (rand() % 3 - 1);
                if(getX < 0){
                    getX = 0;
                }if(boardWidth <= getX){
                    getX = boardWidth-1;
                }
                if(getY < 0){
                    getY = 0;
                }if(boardHeight <= getY){
                    getY = boardHeight-1;
                }
            }else{/*人間*/
                scanf("%d%d", &getX, &getY);
            }
            if(getX == -99 && getY == -99){
                printf("強制終了しますか？(y/n) >> ");
                char answer;
                scanf("%c", &answer);
                scanf("%c", &answer);/*直前のEnterを食べる*/
                if(answer == 'y'){
                    printf("終了します");
                    return 0;
                }
            }else if(!(0 <= getX && getX < boardWidth && 0 <= getY && getY < boardHeight)){
                /*indexが範囲外*/
                if(fShowRequest){printf("x: 0 ~ %d y: 0 ~ %d の範囲で座標を指定してください\n", boardWidth-1, boardHeight-1);};
            }else if(mass[getY][getX] == emptyMass){
                /*そのマスに何もない場合、置いて1枚でもめくれるかどうかをチェック*/
                if(PlaceDisk(mass, getX, getY, boardWidth, boardHeight, disk, iTurn % 2, emptyMass, 0)){
                    /*置けた(ひっくり返せた)のでOK*/
                    if(fAuto[iTurn%2]){
                        printf("%d %d\n", getX, getY);
                    }
                    break;
                }else{
                    if(fShowRequest){printf("1枚もめくれない場所には置けません。\n");};
                }
            }else{
                /*既にそこにコマがある場合、聞き直す*/
                if(fShowRequest){printf("そこにはすでに %c のコマがあります\n", mass[getY][getX]);};
            }
        }while(1);

        /*一番端っこに新たにおかれた場合、端っこの値を更新*/
        if(getX < leftSideDiskPos){
            leftSideDiskPos = getX;
        }if(rightSideDiskPos < getX){
            rightSideDiskPos = getX;
        }if(getY < topSideDiskPos){
            topSideDiskPos = getY;
        }if(bottomSideDiskPos < getY){
            bottomSideDiskPos = getY;
        }

        printf("\n");
    }

    if(fBoardHidden){/*試合中盤面を表\示しなかった場合、結果の盤面だけを表\示*/
        ShowBoard(mass, boardWidth, boardHeight);
        printf("\n");
    }

    /*集計*/
    int nDiskCount[2] = {0, 0};
    for(int y = 0; y < boardHeight; y++){
        for(int x = 0; x < boardWidth; x++){
            for(int n = 0; n < 2; n++){
                if(mass[y][x] == disk[n]){
                    nDiskCount[n]++;
                }
            }
        }
    }

    /*リザルトを盤面で表\示*/
    ShowResultBoard(boardWidth, boardHeight, nDiskCount, disk, 2, nResultShowTime, emptyMass);
    
    /*リザルトを表\示*/
    printf("\n");
    ShowMoveText("--結果--", boardWidth, 800, 400);
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

    /*解放*/
    for(int y = 0; y < boardHeight; y++){
        free(mass[y]);
        mass[y] = NULL;
    }
    free(mass);
    mass = NULL;

    return 0;
}


