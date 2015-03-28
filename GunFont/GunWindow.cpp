/***********************************************
*                                              *
*    За основу взят код NeHe OpenGL BaseCode   *
*            http://nehe.gamedev.net           *
*                     2001                     *
*                                              *
***********************************************/
/*********************************************************************************
*                                                                                *
*	GUNgine версии 0.1 by Mike "CybeRUS" Samsonov  (c) 2002                      *
*   E-mail: mike@bsu.ru                                                          *
*   ICQ UIN: 521412 (at Work), 3742275 (at Home)                                 *
*                                                                                *
*********************************************************************************/

#include "GunWindow.h"													// Заголовочный файл для GunWindow
#include "GunRender.h"													// Заголовочный файл для GunRender
#include "GunTimer.h"

#pragma warning( disable : 4311 )
#pragma warning( disable : 4312 )

GunWindow::GunWindow(void)
{
}

GunWindow::~GunWindow(void)
{
}

static BOOL g_isProgramLooping;											// Переменная цикла создания окна, для переключения FullScreen/Windowed
static BOOL g_createFullScreen;											// Если TRUE, тогда Fullscreen

void TerminateApplication (GL_Window* window)							// Заканчиваем приложение
{
	PostMessage (window->hWnd, WM_QUIT, 0, 0);							// Посылаем сообщение A WM_QUIT
	g_isProgramLooping = FALSE;											// Остановка основного цикла
}

void ToggleFullscreen (GL_Window* window)								// Переключение Fullscreen/Windowed
{
	PostMessage (window->hWnd, WM_TOGGLEFULLSCREEN, 0, 0);				// Посылаем сообщениеA WM_TOGGLEFULLSCREEN 
}

void ReshapeGL (int width, int height)									// Пересчитать настройки окна, после переещения или изменения размера
{
	MainRender.Reshape(width, height);
}

BOOL ChangeScreenResolution (int width, int height, int bitsPerPixel)	// Меняет разрешение экрана
{
	DEVMODE dmScreenSettings;											// Режим устройства
	ZeroMemory (&dmScreenSettings, sizeof (DEVMODE));					// Убедитесь что всё очищено
	dmScreenSettings.dmSize				= sizeof (DEVMODE);				// Размер структуры Devmode
	dmScreenSettings.dmPelsWidth		= width;						// Ширина экрана
	dmScreenSettings.dmPelsHeight		= height;						// Высота экрана
	dmScreenSettings.dmBitsPerPel		= bitsPerPixel;					// Бит на пиксель
	dmScreenSettings.dmFields			= DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
	if (ChangeDisplaySettings (&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
	{
		return FALSE;													// Если не удалось установить режим экрана, то возвращаем False
	}
	return TRUE;														// Всё получилось, возвращаем True
}

BOOL CreateWindowGL (GL_Window* window)									// Этот код создаёт наше OpenGL окно
{
	DWORD windowStyle = WS_OVERLAPPEDWINDOW;							// Зададим стиль окна
	DWORD windowExtendedStyle = WS_EX_APPWINDOW;						// Зададим расширеный стиль окна

	RECT windowRect = {0, 0, window->init.width, window->init.height};	// Зададим координаты окна

	if (window->init.isFullScreen == TRUE)								// Запрошен Fullscreen, попробуем сменить видео режим
	{
		if (ChangeScreenResolution (window->init.width, window->init.height, window->init.bitsPerPixel) == FALSE)
		{
			// Режим Fullscreen не проканал.  Запускаемся в оконно тогда
			MessageBox (HWND_DESKTOP, "Mode Switch Failed.\nRunning In Windowed Mode.", "Error", MB_OK | MB_ICONEXCLAMATION);
			window->init.isFullScreen = FALSE;							// Установим isFullscreen в False (Оконный режим)
		}
		else															// Иначе (Если Fullscreen режим установился)
		{
			ShowCursor (FALSE);											// Выключаем курсор мыши
			windowStyle = WS_POPUP;										// Устанавливаем WindowStyle в WS_POPUP (Всплывающее окно)
			windowExtendedStyle |= WS_EX_TOPMOST;						// Устанавливаем расширеный стиль окна в WS_EX_TOPMOST
		}																// (Top окно покрывает всё другое собой)
	}
	else																// Если Fullscreen небыл выбран
	{
		// Выравним окно до самых границ
		AdjustWindowRectEx (&windowRect, windowStyle, 0, windowExtendedStyle);
	}

	// Сделаем OpenGL окно
	window->hWnd = CreateWindowEx (windowExtendedStyle,					// Расширеный стиль
								   window->init.application->className,	// Имя класса
								   window->init.title,					// Заголовок окна
								   windowStyle,							// Стиль окна
								   0, 0,								// X,Y позиция окна
								   windowRect.right - windowRect.left,	// Ширина окна
								   windowRect.bottom - windowRect.top,	// Высота окна
								   HWND_DESKTOP,						// Родитель окна рабочий стол
								   0,									// Нет меню
								   window->init.application->hInstance, // Прошел Window Instance
								   window);

	if (window->hWnd == 0)												// Окно создалось успешно?
	{
		return FALSE;													// Если нет, то возращаем False
	}

	window->hDC = GetDC (window->hWnd);									// Созраняем контекст устройства для этого окна
	if (window->hDC == 0)												// Контекст устройства сохранился?
	{
		// Нет
		DestroyWindow (window->hWnd);									// Уничтожаем окно
		window->hWnd = 0;												// Обнуляем дискриптор окна
		return FALSE;													// Возвращаем False
	}

	//Устанавливаем OpenGL, если нет то выходим.
	if (!MainRender.SetupOpenGL(window)) return FALSE;

	ShowWindow (window->hWnd, SW_NORMAL);								// Сделаем окно видимым
	window->isVisible = TRUE;											// Установим isVisible в True

	ReshapeGL (window->init.width, window->init.height);				// Подгоним наше окно

	ZeroMemory (window->keys, sizeof (Keys));							// Очищаем все клавиши

	return TRUE;														// Создание окна прошло успешно
																		// Инициализация закончится в WM_CREATE
}

// Обработка сообщений
LRESULT CALLBACK WindowProc (HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// Получаем контекст окна
	GL_Window* window = (GL_Window*)(GetWindowLong (hWnd, GWL_USERDATA));

	switch (uMsg)														// Обрабатываем сообщение
	{
		case WM_SYSCOMMAND:												// Перехватываем системную команду
		{
			switch (wParam)												// Проверяем системные вызовы
			{
				case SC_SCREENSAVE:										// Хранитель экрана пытается запуститься?
				case SC_MONITORPOWER:									// Монитор вытается уйти в спящий режим?
				return 0;												// Не надо этого всёго, пусть ничего не происходит
			}
			break;														// выход
		}
		return 0;														// выходим из функции

		case WM_CREATE:													// Создание окна
		{
			CREATESTRUCT* creation = (CREATESTRUCT*)(lParam);			// Сохраняем структуру окна в указателе
			window = (GL_Window*)(creation->lpCreateParams);
			SetWindowLong (hWnd, GWL_USERDATA, (LONG)(window));
		}
		return 0;														// выходим из функции

		case WM_CLOSE:													// Закрываем окно
			TerminateApplication(window);								// Останавливаем приложение
		return 0;														// выходим из функции

		case WM_SIZE:													// Обнаружена смена размера окна
			switch (wParam)												// Вычислим какая
			{
				case SIZE_MINIMIZED:									// Окно минимизировано?
					window->isVisible = FALSE;							// Устанавливаем isVisible в False
				return 0;												// выходим из функции

				case SIZE_MAXIMIZED:									// Окно максимизировано?
					window->isVisible = TRUE;							// Устанавливаем isVisible в True
					ReshapeGL (LOWORD (lParam), HIWORD (lParam));		// Подгоняем окно - LoWord=Width, HiWord=Height
				return 0;												// выходим из функции

				case SIZE_RESTORED:										// Окно было востановлено?
					window->isVisible = TRUE;							// Устанавливаем isVisible в True
					ReshapeGL (LOWORD (lParam), HIWORD (lParam));		// Подгоняем - LoWord=Width, HiWord=Height
				return 0;												// выходим из функции
			}
		break;															// выходим

		case WM_KEYDOWN:												// Обновляем буффер клавиатуры, нажата клавиша
			if ((wParam >= 0) && (wParam <= 255))						// Клавиша (wParam) в правильном диапазоне?
			{
				window->keys->keyDown [wParam] = TRUE;					// Устанавливаем выбраную клавишу (wParam) в True
				ProcessKeys(hWnd,uMsg,wParam,lParam);
				return 0;												// выходим из функции
			}
		break;															// выход

		case WM_KEYUP:													// Клавиша отпущена
			if ((wParam >= 0) && (wParam <= 255))						// Клавиша (wParam) в правильном диапазоне?
			{
				window->keys->keyDown [wParam] = FALSE;					// Устанавливаем выбраную клавишу (wParam) в False
				return 0;												// выходим из функции
			}
		break;															// выходим

		case WM_TOGGLEFULLSCREEN:										// Меняем видео режим: FullScreen режим вкл./выкл.
			g_createFullScreen = (g_createFullScreen == TRUE) ? FALSE : TRUE;
			PostMessage (hWnd, WM_QUIT, 0, 0);
		break;															// выходим
	}

	return DefWindowProc (hWnd, uMsg, wParam, lParam);					// Остальные сообщения отсылаем в DefWindowProc
}

BOOL RegisterWindowClass (Application* application)						// Регистрирует класс окна для нашего приложения
{																		// TRUE значит всё нормально
	// Регистрируем оконный класс
	WNDCLASSEX windowClass;												// Класс окна
	ZeroMemory (&windowClass, sizeof (WNDCLASSEX));						// Убедимся что память очищена
	windowClass.cbSize			= sizeof (WNDCLASSEX);					// Размер структуры windowClass
	windowClass.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Перерисовываем окно при любых движениях и изменении размера
	windowClass.lpfnWndProc		= (WNDPROC)(WindowProc);				// диксриптор WindowProc сообщений
	windowClass.hInstance		= application->hInstance;				// Устанавливаем Instance
	windowClass.hbrBackground	= (HBRUSH)(COLOR_APPWORKSPACE);			// Класс цвета фона
	windowClass.hCursor			= LoadCursor(NULL, IDC_ARROW);			// Загружаем стрелочку (курсор мыши)
	windowClass.lpszClassName	= application->className;				// Устанавливаем имя класса приложения
	if (RegisterClassEx (&windowClass) == 0)							// Регистрация провалилась?
	{
		// УКАЗАНИЕ: Ошибка не должна никогда появляться 
		MessageBox (HWND_DESKTOP, "RegisterClassEx Failed!", "Error", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;													// Возвращаем False (Ошибка)
	}
	return TRUE;														// Возвращаем True (Всё ок)
}

// Точка входа программы (WinMain)
int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	Application			application;									// Структура приложения
	GL_Window			window;											// Структура окна
	Keys				keys;											// Структура клавиш
	BOOL				isMessagePumpActive;							// Сообщения активны?
	MSG					msg;											// Структура оконных сообщений
	

	// Инициализируем настройки программы
	InitSettings();

	// Заполняем данные приложения
	application.className = "GUNgine";									// Имя класса приложения
	application.hInstance = hInstance;									// Instance приложения


	// Заполняем окно
	ZeroMemory (&window, sizeof (GL_Window));							// Убедимся что память очищена
	window.keys					= &keys;								// Структура клавиш
	window.init.application		= &application;							// Приложение
	window.init.title			= (char *)MainSettings.TITLE.data();	// Заголовок окна
	window.init.width			= MainSettings.SCREEN_WIDTH;			// Ширина окна
	window.init.height			= MainSettings.SCREEN_HEIGHT;			// Высота окна
	window.init.bitsPerPixel	= MainSettings.BPP;						// Битов на пиксель
	window.init.isFullScreen	= MainSettings.FULLSCREEN;				// Полноэкран? (Если TRUE)

	ZeroMemory (&keys, sizeof (Keys));									// Очищаем структуру клавиш

	// Регистрируем класс для нашего окна
	if (RegisterWindowClass (&application) == FALSE)					// Регистраци провалилась?
	{
		// Ошибка
		MessageBox (HWND_DESKTOP, "Error Registering Window Class!", "Error", MB_OK | MB_ICONEXCLAMATION);
		return -1;														// Завершаем приложение
	}

	g_isProgramLooping = TRUE;											// Программный цикл устанавливаем в TRUE
	g_createFullScreen = window.init.isFullScreen;						// g_createFullScreen устанавливаем по выбраному

	// Инициализация опериций со временем (счетчики и т.д.)
	gunInitTimeOperation();
    gunTimer myTimer; // Наш счетчик
	myTimer.gunStartTiming();				// Стартуем счетчик

	while (g_isProgramLooping)											// Цикл пока WM_QUIT не получено
	{
		// Создаём окно
		window.init.isFullScreen = g_createFullScreen;					// Устанавливаем начальные параметры создания окна в Fullscreen?
		if (CreateWindowGL (&window) == TRUE)							// Окно создалось успешно?
		{
			// И так мы имеем окно с уже инициализарованым OpenGL
			if (Initialize (&window, &keys) == FALSE)					// Запускаем инициализацию игры
			{
				// Ошибка
				TerminateApplication (&window);							// Закрываем окно, это повлечет закрытие приложения
			}
			else														// Иначе (Стартуем обработку сообщений)
			{	// Инициализация прошла успешно
				isMessagePumpActive = TRUE;								// Устанавливаем isMessagePumpActive в TRUE
				while (isMessagePumpActive == TRUE)						// Пока обработка сообщений активна
				{
					// Удачное создание окна, проверяем сообщения
					if (PeekMessage (&msg, window.hWnd, 0, 0, PM_REMOVE) != 0)
					{
						// Проверяем на сообщение WM_QUIT
						if (msg.message != WM_QUIT)						// Это сообщение A WM_QUIT?
						{
							DispatchMessage (&msg);						// Если нет, обработаем сообщение
						}
						else											// Иначе (Если сообщение WM_QUIT)
						{
							isMessagePumpActive = FALSE;				// Останавливаем обработку сообщений
						}
					}
					else												// Если нету сообщений
					{
						if (window.isVisible == FALSE)					// Если окно не видимо
						{
							WaitMessage ();								// Окно минимизировано, ждем сообщений
						}
						else											// Если окно видимо
						{
							// Делаем основное
							// Останавливаем счетчик, прошедшее время запишется в myTimer.ElapsedTime
							myTimer.gunTimeElapsed();   
							Update (myTimer.ElapsedTime);	// Обновляем, двигаем и т.д.
							myTimer.gunStartTiming();		// Стартуем счетчик
							Draw ();						// Рисуем сцену
							// Увеличиваем номер кадра
							MainRender.Frame_Num++;
							

							SwapBuffers (window.hDC);					// Меняем буферы (Двойная буфферизация)
						}
					}
				}														// Цикл пока isMessagePumpActive == TRUE
			}															// Если (Initialize (...

			// Приложение заканчивает работу, деинициализация данных
			Deinitialize ();											// Определенное пользователем функция

			MainRender.DestroyWindowGL (&window);									// Уничтожаем активное окно
		}
		else															// Если создание окна провалилось
		{
			// Ошибка создания окна
			MessageBox (HWND_DESKTOP, "Error Creating OpenGL Window", "Error", MB_OK | MB_ICONEXCLAMATION);
			g_isProgramLooping = FALSE;									// Завершаем цикл
		}
	}																	// Пока (isProgramLooping)

	UnregisterClass (application.className, application.hInstance);		// Удаляем класс окна
	return 0;
}																		// Конец WinMain()
