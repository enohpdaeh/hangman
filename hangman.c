#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

void chop(char *cp)
{
  for (; *cp != '\n' && *cp != '\0'; cp++);
  *cp = '\0';
}

int randword(char *wp, int *wc)
{

  FILE  *fp;
  char buf[512];
  char *cp;
  time_t tloc;
  long rval;
  int count;

  if ((fp = fopen("/usr/share/dict/words", "r")) == NULL) {
    fprintf(stderr, "Can not open: /usr/share/dict\n");
    exit(-1);
  }

  srandom(time(&tloc));

  rval = random();
  rval = rval % 235886;

  count = rval;
  while(1) {
    if ((cp = fgets(buf, sizeof(buf), fp)) == NULL) {
      fprintf(stderr, "No more words\n");
      fclose(fp);
      exit(1);
    }
    if (count-- > 0) continue;
    
    chop(cp);
    strcpy(wp, cp);
    *wc = strlen(wp);
    fclose(fp);
    return(1);
  }
  fclose(fp);
  return(0);
}

/*
 * prword関数は、wpの中に格納されている単語を先頭から1文字づつ処理し、
 * もし、wsの該当する文字の場所が、１だったら、単語のその場所の文字を出力し、
 * もし、0だったら、ｰ　を出力する。
 * また、この処理をしながら、当たった文字（開いた文字）の数をカウントし、
 * その値を関数の返値とする
 */
int prword(char *wp, int *ws, int wc)
{
  int i;
  int ret = 0;

  for (i = 0; i < wc; i++, ws++, wp++) {
    if (*ws == 0) {
      printf("-");
    } else {
      ret ++;
      printf("%c", *wp);
    }
  }
  printf("\n");
  return ret;
}
 
void chkword(char *wp, int *ws, int wc, char c)
{
  int i;
  int upc;
  /*
   * 大文字もチェックするために、事前に大文字もupcとして作っておく
   */
  if (islower(c)) {
    upc = c - 'a' + 'A';
  }

  /*
   * wp(単語）の先頭から、1文字筒 c と比較し、もし同じ文字だったら
   * ws(あてられているか状態を管理するための配列wsの値を１：すなわち、
   * 当たったという状態にする
   * このfor文は、単語の文字数だけ繰り返され、wpおよびwsのポインタも
   * 毎回１つづつ先に進めていく
   */
  for (i = 0; i < wc; i++, wp++, ws++) {
    if (*wp == c) {
      *ws = 1;
    }  else if (*wp == upc) {
      *ws = 1;
    }
  }
}
 

int main()
{
  char word[128];
  int  wst[128];
  int wc;
  int ret;
  int i;
  char c;

  /* 単語の取得 */
  randword(word, &wc);

  /* 状態配列の初期化 */
  for(i = 0; i < 128; i++) 
    wst[i] = 0;

    
  while(1) {
  ret = prword(word, wst, wc); /* wordの出力、表示された文字数が返り値 */
  if (ret >= wc) break;    /* すべての文字が表示されたら終了　 */
     c = fgetc(stdin);      /* １文字読み込む */
      /* 読み込まれた文字が改行コードだったら、次の1文字の入力をおこなう。 */
     if (c == '\n') { 
       c = fgetc(stdin);
     }
     chkword(word, wst, wc, c); /* 入力された文字が単語の中にあるかのチェック */
  }
}
