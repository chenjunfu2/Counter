﻿#include <stdio.h>
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

HANDLE hi, ho;

void ConsoleShowCursor(bool bShow)//隐藏控制台光标
{
	CONSOLE_CURSOR_INFO info = { 1,bShow };
	SetConsoleCursorInfo(ho, &info);
}

void ConsoleSetCursorPos(const COORD &crdPos)
{
	SetConsoleCursorPosition(ho, crdPos);
}

COORD ConsoleGetCursorPos(void)
{
	CONSOLE_SCREEN_BUFFER_INFO screen_buffer_info;
	GetConsoleScreenBufferInfo(ho, &screen_buffer_info);
	return screen_buffer_info.dwCursorPosition;
}

void ConsoleClearScreen(void)
{
	// 获取控制台屏幕缓冲区信息
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(ho, &csbi);

	// 计算需要填充的字符数
	DWORD cellCount = csbi.dwSize.X * csbi.dwSize.Y;

	// 填充控制台缓冲区
	COORD homeCoords = { 0, 0 }; // 从 (0, 0) 开始填充
	DWORD count;
	FillConsoleOutputCharacterA(ho, ' ', cellCount, homeCoords, &count); // 用空格填充
	FillConsoleOutputAttribute(ho, csbi.wAttributes, cellCount, homeCoords, &count); // 恢复属性

	// 将光标移动到 (0, 0)
	ConsoleSetCursorPos(homeCoords);
}

void ConsolePause(void)
{
	printf("按任意键继续...\n");
	//等待输入
	INPUT_RECORD ir;
	DWORD tmp;
	do
	{
		ReadConsoleInputA(hi, &ir, 1, &tmp);
	} while (ir.EventType != KEY_EVENT || ir.Event.KeyEvent.bKeyDown != false);
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

//unsigned short GetKeyVal(void)
//{
//	unsigned short usKeyVal = 0;
//	unsigned char ucGet = _getch();
//	if (ucGet == 0x00)
//	{
//		ucGet = _getch();//reget
//		usKeyVal = 0x00 << 8 | ucGet;
//	}
//	else if (ucGet == 0xE0)
//	{
//		ucGet = _getch();//reget
//		usKeyVal = 0xE0 << 8 | ucGet;
//	}
//	else
//	{
//		usKeyVal = 0xFF << 8 | ucGet;
//	}
//
//	return usKeyVal;
//}

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
	hi = GetStdHandle(STD_INPUT_HANDLE);
	ho = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleMode(ho, ENABLE_VIRTUAL_TERMINAL_PROCESSING | ENABLE_PROCESSED_OUTPUT);//允许使用ASCII控制序列、VT100和类似控制字符序列
	SetConsoleMode(hi, ENABLE_MOUSE_INPUT | ENABLE_WINDOW_INPUT | ENABLE_EXTENDED_FLAGS/* | ENABLE_QUICK_EDIT_MODE*/);//接受鼠标输入事件、窗口改变事件，不允许用户复制选择内容
	
	ConsoleShowCursor(false);

	printf(
		"简易计数器\n"
		"=================\n"
		"加一 -> Space/LMB\n"
		"减一 -> Backspace/RMB\n"
		"修改 -> Enter\n"
		"清零 -> Delete\n"
		"退出 -> Esc\n"
		"=================\n"
	);
	
	long long llCount = 0;
	FILE *f = OpenDat("Count.dat");
	ReadValue(llCount, f);//读取之前的计数值

	ConsolePause();
	ConsoleClearScreen();

	bool bZeroConfirm = false;
	bool bExitConfirm = false;
	bool bChange = false;
	bool bMLB = false, bMRB = false;
	//绘制数值
	PrintCount(llCount);
	while (true)
	{
		//等待输入
		INPUT_RECORD ir;
		DWORD tmp;
		ReadConsoleInputA(hi, &ir, 1, &tmp);

		//如果是鼠标点击消息，则转换为键盘消息处理
		if (ir.EventType == MOUSE_EVENT && (ir.Event.MouseEvent.dwEventFlags == 0 || ir.Event.MouseEvent.dwEventFlags == DOUBLE_CLICK))//处理单击和双击消息
		{
			if ((ir.Event.MouseEvent.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED) == FROM_LEFT_1ST_BUTTON_PRESSED)
			{
				bMLB = true;
			}
			else if ((ir.Event.MouseEvent.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED) == 0 && bMLB == true)
			{
				bMLB = false;
				ir.EventType = KEY_EVENT;
				ir.Event.KeyEvent.bKeyDown = false;
				ir.Event.KeyEvent.wVirtualKeyCode = VK_SPACE;
			}

			if ((ir.Event.MouseEvent.dwButtonState & RIGHTMOST_BUTTON_PRESSED) == RIGHTMOST_BUTTON_PRESSED)
			{
				bMRB = true;
			}
			else if ((ir.Event.MouseEvent.dwButtonState & RIGHTMOST_BUTTON_PRESSED) == 0 && bMRB == true)
			{
				bMRB = false;
				ir.EventType = KEY_EVENT;
				ir.Event.KeyEvent.bKeyDown = false;
				ir.Event.KeyEvent.wVirtualKeyCode = VK_BACK;
			}
		}

		if (ir.EventType == KEY_EVENT && ir.Event.KeyEvent.bKeyDown == false)//keyup才处理
		{
			bChange = true;//假设有修改
			switch (ir.Event.KeyEvent.wVirtualKeyCode)
			{
			case VK_SPACE://空格 加一
				++llCount;
				break;
			case VK_BACK://退格 减一
				--llCount;
				break;
			case VK_RETURN://回车 输入
				printf("请输入数值:");
				while (true)
				{
					SetConsoleMode(hi, ENABLE_ECHO_INPUT | ENABLE_LINE_INPUT | ENABLE_PROCESSED_INPUT | ENABLE_EXTENDED_FLAGS | ENABLE_QUICK_EDIT_MODE);
					ConsoleShowCursor(true);
					long long llNew = 0;
					int iRet = scanf("%lld", &llNew);
					while (getchar() != '\n') continue;
					ConsoleShowCursor(false);
					SetConsoleMode(hi, ENABLE_MOUSE_INPUT | ENABLE_WINDOW_INPUT | ENABLE_EXTENDED_FLAGS);

					if (iRet != 1)
					{
						printf("输入错误，请重新输入:");
						continue;
					}

					llCount = llNew;
					ConsoleClearScreen();
					break;
				}
				break;
			case VK_DELETE://DEL 清零
				if (bZeroConfirm)
				{
					llCount = 0;
				}
				else
				{
					bZeroConfirm = true;
					printf("请再次键入DEL以确认清零\n");
					continue;//跳过后面设置为false的语句再次等待输入
				}
				break;
			case VK_ESCAPE://ESC 退出
				if (bExitConfirm)
				{
					return 0;
				}
				else
				{
					bExitConfirm = true;
					printf("请再次键入ESC以确认退出\n");
					continue;//跳过后面设置为false的语句再次等待输入
				}
				break;
			default:
				bChange = false;//实际上并没有，假设失败
				break;
			}

			//总是设置为false
			if (bZeroConfirm || bExitConfirm)
			{
				bZeroConfirm = false;
				bExitConfirm = false;
				ConsoleClearScreen();//清屏
			}

			if (bChange)
			{
				//写入数值
				WriteValue(llCount, f);
				//绘制数值
				PrintCount(llCount);
			}
		}
		else if (ir.EventType == WINDOW_BUFFER_SIZE_EVENT)//窗口改变，重新隐藏光标
		{
			ConsoleShowCursor(false);
		}
	}
	return 0;
}