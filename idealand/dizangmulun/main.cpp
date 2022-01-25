#include <stdio.h>
#include <stdlib.h>

const int  Mian = 4;   // 每个木轮的面数
const int  Lunc = 6;   // 共6个木轮
const int  Zhic = 3;   // 共3次投掷
int* Ls[Lunc];       // 六个木轮上的数字


const int Yic = 64;   // 一次投掷共有64种和数,最大值为18+15+12+9+6+3=3*(1+..+6)=3*7*3=63，最小值为0
int Yi[Yic];          // 64种和数的统计次数
int  Yip[Lunc];       // 一次投掷时，六个木轮的当前相位  

const int Hec = 190;  // 3次投掷共有190种和数
int He[Hec];          // 190种和数的统计次数
int  Hep[Zhic];       // 3次投掷的当前相位

int Pai[Hec];         // 190种和数的次数排行

char* Desc[Hec];      // 190种和数的意义


// 填充每个轮子上的数字，初始化数组
void init()
{
  for (int i = 0; i < Lunc; i++)
  {
    int base = 3 * i;
    int* L = (int*)malloc(sizeof(int) * Mian); Ls[i] = L;
    for (int j = 0; j < Mian; j++) L[j] = (j == 0 ? 0 : base + j);
    Yip[i] = 0;
  }
  for (int i = 0; i < Yic; i++) Yi[i] = 0;
  for (int i = 0; i < Hec; i++) { He[i] = 0; Pai[i] = -1; }
  for (int i = 0; i < Zhic; i++) Hep[i] = 0;
}


/* 一次投掷第i个轮子翻面 */
int nextMian(int i)
{
  Yip[i]++; if (Yip[i] >= Mian)
  {
    // if (i <= 3) { printf("轮位："); for (int j = 0; j < Lunc; j++) printf("%d ", Yip[j]); printf("\n"); }
    if (i <= 0) { printf("\n一次投掷统计完毕\n"); return -1; } // 已历遍
    Yip[i] = 0; return nextMian(i - 1); // 进位
  }
  return 0; // 还有下一个和数
}


/* 3次投掷第i次进位 */
int nextHe(int i)
{
  Hep[i]++; if (Hep[i] >= Yic)
  {
    // if (i <= 1) { printf("和位："); for (int j = 0; j < Zhic; j++) printf("%d ", Hep[j]); printf("\n"); }
    if (i <= 0) return -1; // 已历遍
    Hep[i] = 0; return  nextHe(i - 1);// 进位
  }
  return 0; // 还有下一个和数
}



/* 3次投掷和数排序 */
void sort()
{
  int end = 0; // 第一个-1
  for (int i = 0; i < Hec; i++)
  {
    int c = He[i], inserted = 0;
    for (int j = 0; j < end; j++)
    {
      if (He[Pai[j]] < c)
      {
        for (int k = end - 1; k >= j; k--)
        {
          Pai[k + 1] = Pai[k];
        }
        Pai[j] = i; end++; inserted = 1; break;
      }
    }
    if (!inserted) Pai[end++] = i;
  }
}

void print()
{
  printf("\n第三轮相概率排行榜\n");
  long long int total = 0; // 总次数
  for (int i = 0; i < Hec; i++)
  {
    total += He[i];
  }

  Desc[112] = (char*)"所向處可開化";
  Desc[113] = (char*)"所向處自獲利";
  Desc[114] = (char*)"所遊路無惱害";
  Desc[115] = (char*)"所遊路有惱害";
  Desc[116] = (char*)"君民惡饑饉起";
  Desc[117] = (char*)"君民惡多疾疫";
  Desc[118] = (char*)"君民好國豐樂";
  Desc[119] = (char*)"君無道國災亂";
  Desc[120] = (char*)"君修德災亂滅";
  Desc[121] = (char*)"君行惡國將破";
  Desc[122] = (char*)"君修善國還立";
  Desc[123] = (char*)"觀所避得度難";
  Desc[124] = (char*)"觀所避不脫難";
  Desc[125] = (char*)"所住處眾安隱";
  Desc[126] = (char*)"所住處有障難";
  Desc[127] = (char*)"所依聚眾不安";
  Desc[128] = (char*)"閑靜處無諸難";
  Desc[129] = (char*)"觀怪異無損害";
  Desc[130] = (char*)"觀怪異有損害";
  Desc[131] = (char*)"觀怪異精進安";
  Desc[132] = (char*)"觀所夢無損害";
  Desc[133] = (char*)"觀所夢有所損";
  Desc[134] = (char*)"觀所夢精進安";
  Desc[135] = (char*)"觀所夢為吉利";
  Desc[136] = (char*)"觀障亂速得離";
  Desc[137] = (char*)"觀障亂漸得離";
  Desc[138] = (char*)"觀障亂不能離";
  Desc[139] = (char*)"觀障亂一心除";
  Desc[140] = (char*)"觀所難速得脫";
  Desc[141] = (char*)"觀所難久得脫";
  Desc[142] = (char*)"觀所難受衰惱";
  Desc[143] = (char*)"觀所難精進脫";
  Desc[144] = (char*)"觀所難命當盡";
  Desc[145] = (char*)"觀所患大不調";
  Desc[146] = (char*)"觀所患非人惱";
  Desc[147] = (char*)"觀所患合非人";
  Desc[148] = (char*)"觀所患可療治";
  Desc[149] = (char*)"觀所患難療治";
  Desc[150] = (char*)"觀所患精進差";
  for (int i = 0; i < Hec; i++)
  {
    int he = Pai[i]; long long int ci = He[he];
    printf("%3d. %1.9f%%(%10lld) %3d : %s\n", i + 1, ci * 100.0 / total, ci, he, Desc[he] == NULL ? "" : Desc[he]);
  }
}


int dizangmulu3()
{
  printf("南无地藏菩萨摩诃萨！\n");
  init();

  /* 一次投掷的和数 */
  do {
    int sum = 0;
    for (int i = 0; i < Lunc; i++)
    {
      // printf("第%d个轮子的当前位置为%d, 数字为%d\n", i, Yip[i], Ls[i][Yip[i]]);
      sum += Ls[i][Yip[i]];
    }
    // printf("sum = %d\n", sum);
    Yi[sum]++;
  } while (nextMian(Lunc - 1) >= 0);


  /* 3次投掷的和数 */
  do {
    int sum = 0, c = 1;
    for (int i = 0; i < Zhic; i++)
    {
      sum += Hep[i]; c *= Yi[Hep[i]];
    }
    He[sum] += c;
  } while (nextHe(Zhic - 1) >= 0);

  sort(); print();

  return 0;

}


