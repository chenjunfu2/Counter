#include <stdio.h>
#include <conio.h>
#include <Windows.h>


#define COLU_SPACE 1
#define LINE_SPACE 1
#define SPACE_SYMB "  "
#define BLOCK_SYMB "■"


#define NUMX 3
#define NUMY 5
#define NUMC 12
#define NEGS (NUMC-2)//负号
#define SPAC (NUMC-1)//空白
#define LINE -1//新行
#define BEGI -2//开头
constexpr static inline const bool Number[NUMC][NUMY][NUMX] =
{
	{
		{1,1,1},
		{1,0,1},
		{1,0,1},
		{1,0,1},
		{1,1,1},
	},
	{
		{0,0,1},
		{0,0,1},
		{0,0,1},
		{0,0,1},
		{0,0,1},
	},
	{
		{1,1,1},
		{0,0,1},
		{1,1,1},
		{1,0,0},
		{1,1,1},
	},
	{
		{1,1,1},
		{0,0,1},
		{1,1,1},
		{0,0,1},
		{1,1,1},
	},
	{
		{1,0,1},
		{1,0,1},
		{1,1,1},
		{0,0,1},
		{0,0,1},
	},
	{
		{1,1,1},
		{1,0,0},
		{1,1,1},
		{0,0,1},
		{1,1,1},
	},
	{
		{1,1,1},
		{1,0,0},
		{1,1,1},
		{1,0,1},
		{1,1,1},
	},
	{
		{1,1,1},
		{0,0,1},
		{0,0,1},
		{0,0,1},
		{0,0,1},
	},
	{
		{1,1,1},
		{1,0,1},
		{1,1,1},
		{1,0,1},
		{1,1,1},
	},
	{
		{1,1,1},
		{1,0,1},
		{1,1,1},
		{0,0,1},
		{1,1,1},
	},
	{
		{0,0,0},
		{0,0,0},
		{1,1,1},
		{0,0,0},
		{0,0,0},
	},
	{
		{0,0,0},
		{0,0,0},
		{0,0,0},
		{0,0,0},
		{0,0,0},
	},
};

void ConsoleShowCursor(bool bShow)//隐藏控制台光标
{
	CONSOLE_CURSOR_INFO info = { 1,bShow };
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
}

void ConsoleSetCursorPos(const COORD &crdPos)
{
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), crdPos);
}

COORD ConsoleGetCursorPos(void)
{
	CONSOLE_SCREEN_BUFFER_INFO screen_buffer_info;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &screen_buffer_info);
	return screen_buffer_info.dwCursorPosition;
}

void ConsoleClearScreen(void)
{
	system("cls");
}

void ConsolePause(void)
{
	system("pause");
}


void PrintNum(char cNum)//小于0输出负号，大于9输出换行，否则输出数值
{
	COORD cCurPos = ConsoleGetCursorPos();

	if (cNum == LINE)//换行
	{
		ConsoleSetCursorPos({ SHORT(0),SHORT(cCurPos.Y + NUMY + LINE_SPACE) });
		return;
	}
	else if (cNum == BEGI)
	{
		ConsoleSetCursorPos({ SHORT(0),SHORT(0) });
		return;
	}
	else if (cNum < 0 || cNum >= NUMC)
	{
		return;
	}

	for (long y = 0; y < NUMY; ++y)
	{
		ConsoleSetCursorPos({ SHORT(cCurPos.X),SHORT(cCurPos.Y + y) });
		for (long x = 0; x < NUMX; ++x)
		{
			if (Number[(long)cNum][y][x])
			{
				printf(BLOCK_SYMB);
			}
			else
			{
				printf(SPACE_SYMB);
			}
		}
	}

	ConsoleSetCursorPos({ SHORT(cCurPos.X + (NUMX + COLU_SPACE) * 2),SHORT(cCurPos.Y) });
}

void PrintCount(long long llCount)
{
	ConsoleShowCursor(false);
	PrintNum(BEGI);

	if (llCount == 0)
	{
		PrintNum(0);
	}

	unsigned char ucBcd[32];
	if (llCount < 0)
	{
		PrintNum(NEGS);//负号
		llCount = -llCount;
	}

	long lBcdCnt = 0;
	while (llCount > 0)
	{
		ucBcd[lBcdCnt] = llCount % 10;
		llCount /= 10;
		++lBcdCnt;
	}

	while (lBcdCnt > 0)
	{
		--lBcdCnt;
		PrintNum(ucBcd[lBcdCnt]);
	}

	PrintNum(SPAC);//空白
	PrintNum(LINE);//换行
}

unsigned short GetKeyVal(void)
{
	unsigned short usKeyVal = 0;
	unsigned char ucGet = _getch();
	if (ucGet == 0x00)
	{
		ucGet = _getch();//reget
		usKeyVal = 0x00 << 8 | ucGet;
	}
	else if (ucGet == 0xE0)
	{
		ucGet = _getch();//reget
		usKeyVal = 0xE0 << 8 | ucGet;
	}
	else
	{
		usKeyVal = 0xFF << 8 | ucGet;
	}

	return usKeyVal;
}

void ReadValue(long long &llRead, FILE *f)
{
	if (f == NULL)
	{
		return;
	}

	rewind(f);//恢复到文件开头
	fread(&llRead, sizeof(llRead), 1, f);
}

void WriteValue(const long long &llWrite, FILE *f)
{
	if (f == NULL)
	{
		return;
	}

	rewind(f);//恢复到文件开头
	fwrite(&llWrite, sizeof(llWrite), 1, f);
}

FILE *OpenDat(const char *pDatName)
{
	FILE *f;

	//直接打开
	f = fopen(pDatName, "rb+");
	if (f != NULL)
	{
		setvbuf(f, NULL, _IONBF, 0);//无缓冲
		return f;
	}

	//创建文件
	f = fopen(pDatName, "wb+");//打开失败则创建
	if (f == NULL)//失败但是无需退出，告知用户即可
	{
		printf(
			"无法创建数据记录文件，您的计数值不会被保存，\n"
			"请注意当前程序是否有文件创建和读写等权限\n"
		);
	}

	return f;
}


int main(void)
{
	long long llCount = 0;

	printf(
		"简易计数器\n"
		"=================\n"
		"加一 -> Space\n"		//0xFF 0x20
		"减一 -> Backspace\n"	//0xFF 0x08
		"修改 -> Enter\n"		//0xFF 0x0D
		"清零 -> Delete\n"		//0xE0 0x53
		"退出 -> Esc\n"			//0xFF 0x1B
		"=================\n"
	);
	
	FILE *f = OpenDat("Count.dat");
	ReadValue(llCount, f);//读取之前的计数值

	ConsolePause();
	ConsoleClearScreen();

	bool bZeroConfirm = false;
	bool bExitConfirm = false;
	//绘制数值
	PrintCount(llCount);
	while (true)
	{
		//等待输入
		switch (GetKeyVal())
		{
		case 0xFF20://空格+1
			{
				llCount += 1;
			}
			break;
		case 0xFF08://退格-1
			{
				llCount -= 1;
			}
			break;
		case 0xFF0D://回车修改
			{
				printf("请输入数值:");
				while (true)
				{
					ConsoleShowCursor(true);
					long long llNew = 0;
					int iRet = scanf("%lld", &llNew);
					while (getchar() != '\n') continue;
					ConsoleShowCursor(false);

					if (iRet != 1)
					{
						printf("输入错误，请重新输入:");
						continue;
					}

					llCount = llNew;
					ConsoleClearScreen();
					break;
				}
			}
			break;
		case 0xE053://删除清零
			{
				if (bZeroConfirm)
				{
					llCount = 0;
				}
				else
				{
					bZeroConfirm = true;
					printf("请再次键入以确认\n");
					continue;//跳过后面设置为false的语句再次等待输入
				}
			}
			break;
		case 0xFF1B://最小化退出
			{
				if (bExitConfirm)
				{
					return 0;
				}
				else
				{
					bExitConfirm = true;
					printf("请再次键入以确认\n");
					continue;//跳过后面设置为false的语句再次等待输入
				}
			}
			break;
		default:
			continue;
			break;
		}

		//总是设置为false
		if (bZeroConfirm || bExitConfirm)
		{
			bZeroConfirm = false;
			bExitConfirm = false;
			ConsoleClearScreen();//清屏
		}
		
		//写入数值
		WriteValue(llCount, f);
		//绘制数值
		PrintCount(llCount);
	}
	return 0;
}