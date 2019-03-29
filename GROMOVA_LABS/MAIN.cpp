#include "windows.h"

//используем короткий неотрицательный инт, т.к. размеры заданых нами айдишников невелик
//так же использует constexpr, т.к. на этапе компиляции наши константы уже заданы
//и компилятор 
constexpr unsigned short int constantIdintifierOfFileEditing = 100;// Передаем в виндоу хендлер, который задан ниже
/*
остальные константы - задаем в аппенд меню - соответствующие id каждому блоку меню
*/
constexpr unsigned short int constantIdintifierOfFileOpening = 101;
constexpr unsigned short int constantIdintifierOfFileSaving = 102;
constexpr unsigned short int constantIdintifierOfFileExit = 103;



//Главные экземпляры программы - окошко и его дескрипторы
WNDCLASSEX wndClassEX;
HWND globalWndHandle_dscr;
HWND editWndHandle_dscr;

//блок меню (1) и его кнопки (2) 
HMENU menuStructHandle_dscr, menuFieldHandle_dscr;

//шрифт
HFONT fontHandle_dscr;

///Экзампляры, относящиеся к файлу
OPENFILENAME openFileHandling;
HANDLE fileHandler_dscr;
char maxFileNameContainer[260];

//системное сообщение, которое постоянно анализируем, при помощи цикла в ВИНМЕЙН
MSG handlingMsg;


/*
FUNTION DECLARATION BLOCK
*/
LRESULT CALLBACK wndProcess_CALLBACK(HWND, UINT, WPARAM, LPARAM);
BOOL InitializeFieldsOfDescriptors(HINSTANCE, int); //



int WINAPI WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPTSTR    lpCmdLine,
	int       nCmdShow)
{
	if (!InitializeFieldsOfDescriptors(hInstance, nCmdShow))
		return FALSE;

	/*
	ИНФОРМАЦИЯ ИЗ MDSN-а касательно GetMessage
	Retrieves a message from the calling thread's message queue.
	The function dispatches incoming sent messages until a posted message is available for retrieval.
	*/
	while (GetMessage(&handlingMsg, NULL, NULL, NULL))
	{
		//стандартная обработка сообщений в винапи
		TranslateMessage(&handlingMsg);
		DispatchMessage(&handlingMsg);
	}
	return (int)handlingMsg.wParam;
}


/*
FUNTION DEFINITION BLOCK
*/

LRESULT CALLBACK wndProcess_CALLBACK(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	//Требуются для WM_COMMAND
	int windowMessageID, // в зависимости от id будет выполнен тот или иной кейс 
						//(параметры кейсов - заданные нами в начале константы, которыми мы
					   //инициализировали блоки меню
		windowMessageEVENT;
	switch (message)
	{
		/*
			переходим сюда при создании окна
			создаем окно и задаем блоки меню с помощью AppendMenu
		*/
	case WM_CREATE:
	{
		RECT rcClient;// стр-ра, определяющая размер клиентской области (прямоугольня область окна)
		GetClientRect(hWnd, &rcClient);
		editWndHandle_dscr = CreateWindow("EDIT", NULL, WS_CHILD | WS_VISIBLE | WS_VSCROLL |
			ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL, 0, 0, 0, 0,
			hWnd, (HMENU)constantIdintifierOfFileEditing, (HINSTANCE)::GetModuleHandle(NULL), NULL);
		menuStructHandle_dscr = CreateMenu();
		menuFieldHandle_dscr = CreatePopupMenu();
		AppendMenu(menuStructHandle_dscr, MF_STRING | MF_POPUP, (UINT)(LONG_PTR)menuFieldHandle_dscr, "File");
		AppendMenu(menuFieldHandle_dscr, MF_STRING, constantIdintifierOfFileOpening, "Open");
		AppendMenu(menuFieldHandle_dscr, MF_STRING, constantIdintifierOfFileSaving, "Save");
		AppendMenu(menuFieldHandle_dscr, MF_SEPARATOR, 0, NULL);
		AppendMenu(menuFieldHandle_dscr, MF_STRING, constantIdintifierOfFileExit, "Terminate");
		SetMenu(hWnd, menuStructHandle_dscr);

		fontHandle_dscr = CreateFont(12, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, ANSI_CHARSET,
			OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
			DEFAULT_PITCH | FF_SWISS, "Arial");

		SendMessage(editWndHandle_dscr, WM_SETFONT, WPARAM(fontHandle_dscr), TRUE);
		SendMessage(editWndHandle_dscr, EM_SETLIMITTEXT, WPARAM(-1), 0);
		break;
	}
	//Если размер окна изменен ---> 
	case WM_SIZE:
	{
		MoveWindow(editWndHandle_dscr, 0, 0, LOWORD(lParam), HIWORD(lParam), TRUE);
		return 0;
	}
	/*когда что-то в меню выбирается, заходим сюда*/
	case WM_COMMAND:
		windowMessageID = LOWORD(wParam);
		windowMessageEVENT = HIWORD(wParam);
		switch (windowMessageID)
		{
			//ОТКРЫВАЕМ ФАЙЛ
		case constantIdintifierOfFileOpening:
		{
			if (GetOpenFileName(&openFileHandling) == TRUE)
			{
				fileHandler_dscr = CreateFile(openFileHandling.lpstrFile, GENERIC_READ,
					0, (LPSECURITY_ATTRIBUTES)NULL,
					OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,
					(HANDLE)NULL);
				if (fileHandler_dscr == NULL)
				{
					MessageBox(hWnd, "Can't open file", "ERROR", 0x10);
				}
				DWORD nBufLen = ::GetFileSize(fileHandler_dscr, NULL);
				DWORD nBufLenToRead = 0;
				char* szReadBuf = new char[nBufLen];
				if (!ReadFile(fileHandler_dscr, szReadBuf, nBufLen, &nBufLenToRead, NULL))
				{
					MessageBox(hWnd, "Can't read file", "ERROR", 0x10);
				}
				SetWindowText(editWndHandle_dscr, szReadBuf);
				delete[] szReadBuf;
				CloseHandle(fileHandler_dscr);
			}
			break;
		}
		//СОХРАНЯЕМ ФАЙЛ
		case constantIdintifierOfFileSaving:
		{
			if (GetSaveFileName(&openFileHandling) == TRUE)
			{
				fileHandler_dscr = CreateFile(openFileHandling.lpstrFile, GENERIC_WRITE,
					0, (LPSECURITY_ATTRIBUTES)NULL,
					CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL,
					(HANDLE)NULL);
				if (fileHandler_dscr == NULL)
					MessageBox(hWnd, "Can't open file", "ERROR", 0x10);
				DWORD nBufLen = (DWORD)SendMessage(editWndHandle_dscr, WM_GETTEXTLENGTH, 0, 0);
				DWORD nBufLenToWrite = 0;
				char* sizeOfCurrentFile = new char[nBufLen];
				GetWindowText(editWndHandle_dscr, sizeOfCurrentFile, nBufLen);
				if (!WriteFile(fileHandler_dscr, sizeOfCurrentFile, nBufLen, &nBufLenToWrite, NULL))
					MessageBox(hWnd, "Can't write to file", "ERROR", 0x10);
				delete[] sizeOfCurrentFile;
				CloseHandle(fileHandler_dscr);
			}
			break;
		}
		//ЗАКРЫВАЕМ ПРОГРАММУ И УДАЛЯЕМ ОКНО
		case constantIdintifierOfFileExit:
		{
			DestroyWindow(hWnd);
			break;
		}
		default:
			//СТАНДАРТНЫЙ ВОЗВРАТ ОКНА ИЗ КОЛБЕКА ОПРЕДЕЛЕННОГО ПОЛЬЗОВАТЕЛЕМ
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
		//КЕЙС СРАБОТАЕТ ПРИ ЗАКРЫТИЕ ОКОШКА - БУДЕТ ВЫЗВАН ДЕСТРУКТОР ИЛИ ЧТО-ТО ПОДОБНОЕ
	case WM_DESTROY:
		//Сообщение, которе будет выведено при закрытии окна ( принимает int exitCode )
		PostQuitMessage(0);
		break;
	default:
		//СТАНДАРТНЫЙ ВОЗВРАТ ОКНА ИЗ КОЛБЕКА ОПРЕДЕЛЕННОГО ПОЛЬЗОВАТЕЛЕМ
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}






/*Обычная функция инициализации экземпляра класса окна и его регистрации при помощи функции
функции ВИНАПИ RegisterClassEx(WNDCLASSEX param), в которую передаем адрес окна
после инициализации всех важных для работы программы полей


Далее в ядре при помощи соотвутствующей функции создаем дескриптор (обработчик) окна,
который инициализируем при помощи ранее созданного экземпляра WNDCLASSEX

*/
BOOL InitializeFieldsOfDescriptors(HINSTANCE hInstance, int nCmdShow)
{
	//Инициализируем поля

	wndClassEX.cbSize = sizeof(WNDCLASSEX);
	//СТИЛЬ ОКОШКА
	wndClassEX.style = CS_HREDRAW | CS_VREDRAW;
	//УСТАНАВЛИВАЕМ 
	wndClassEX.lpfnWndProc = (WNDPROC)wndProcess_CALLBACK;
	wndClassEX.cbClsExtra = 0;
	wndClassEX.cbWndExtra = 0;
	wndClassEX.hInstance = hInstance;
	wndClassEX.hIcon = NULL;
	wndClassEX.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClassEX.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wndClassEX.lpszMenuName = NULL;
	wndClassEX.lpszClassName = "FRM_TEXTEDIT";
	wndClassEX.hIconSm = NULL;


	//Функция RegisterClassEx регистрирует класс окна для последующего использования при вызове функции CreateWindow или CreateWindowEx.
	//***************************//
	RegisterClassEx(&wndClassEX);
	globalWndHandle_dscr = CreateWindow(wndClassEX.lpszClassName, "Sublime Text -3", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

	//проверяем, успещно ли создался дескриптор окна при помощи CreateWindow
	//если параметры указаны неверно - программа произведет аварийное завершение, вернув 0 в функцию winmain
	//тем самым вызвав terminate
	//***************************//
	if (!globalWndHandle_dscr)
		return FALSE;
	ShowWindow(globalWndHandle_dscr, nCmdShow);
	UpdateWindow(globalWndHandle_dscr);


	/*Функция ZeroMemory предназначена для обнуления памяти. Она похожа на функцию memset,
	за исключением того, что последняя может заполнять память любыми значениями.
	Функция же ZeroMemory может заполнять память только нулями.
	У этой функции два параметра: первый определяет, начиная с какого места в памяти мы делаем ее обнуление и второй определяет,
	сколько байт мы обнуляем.*/

	ZeroMemory(&openFileHandling, sizeof(openFileHandling));

	//Инициалихируем поля обработчика файла, так же в lpstrFilter устанавливаем соответствуещее 
	//регулярное выражение для открытия соответствующего формата файлов
	openFileHandling.lStructSize = sizeof(openFileHandling);
	openFileHandling.hwndOwner = globalWndHandle_dscr;
	openFileHandling.lpstrFile = maxFileNameContainer;
	openFileHandling.nMaxFile = sizeof(maxFileNameContainer);
	openFileHandling.lpstrFilter = "All\0*.*\0Text\0*.TXT\0";
	openFileHandling.nFilterIndex = 1;
	openFileHandling.lpstrFileTitle = NULL;
	openFileHandling.nMaxFileTitle = 0;
	openFileHandling.lpstrInitialDir = NULL;
	openFileHandling.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	return TRUE;
}
//╔════╦╗╔╦══╦╗─╔╦╗╔══╗
//╚═╗╔═╣║║║╔╗║╚═╝║║║╔═╝
//──║║─║╚╝║╚╝║╔╗─║╚╝║
//──║║─║╔╗║╔╗║║╚╗║╔╗║
//──║║─║║║║║║║║─║║║║╚═╗
//──╚╝─╚╝╚╩╝╚╩╝─╚╩╝╚══╝
//╔╗╔╦══╦╗╔╗
//║║║║╔╗║║║║
//║╚╝║║║║║║║
//╚═╗║║║║║║║
//─╔╝║╚╝║╚╝║
//─╚═╩══╩══╝
//╔══╦══╦═══╗
//║╔═╣╔╗║╔═╗║
//║╚═╣║║║╚═╝║
//║╔═╣║║║╔╗╔╝
//║║─║╚╝║║║║
//╚╝─╚══╩╝╚╝
//╔══╦════╦════╦═══╦╗─╔╦════╦══╦╗─╔╗
//║╔╗╠═╗╔═╩═╗╔═╣╔══╣╚═╝╠═╗╔═╩╗╔╣╚═╝║
//║╚╝║─║║───║║─║╚══╣╔╗─║─║║──║║║╔╗─║
//║╔╗║─║║───║║─║╔══╣║╚╗║─║║──║║║║╚╗║
//║║║║─║║───║║─║╚══╣║─║║─║║─╔╝╚╣║─║║
//╚╝╚╝─╚╝───╚╝─╚═══╩╝─╚╝─╚╝─╚══╩╝─╚╝