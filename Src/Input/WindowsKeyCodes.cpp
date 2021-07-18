#include "pch.h"
#include "WindowsKeyCodes.h"

namespace scuff3d {



	const std::string keycodeName(const int keycode)
	{
		switch(keycode) {
		case VK_LBUTTON: return "Left mouse";
		case VK_RBUTTON: return "Right mouse";
		case VK_MBUTTON: return "Middle mouse button ";
		case VK_XBUTTON1: return "X1 mouse button";
		case VK_XBUTTON2: return "X2 mouse button";
		case VK_SHIFT: return "Shift";
		case VK_CONTROL: return "Ctrl";
		case VK_MENU: return "Alt";
		case VK_PAUSE: return "Pause";
		case VK_PRIOR: return "Pgup";
		case VK_NEXT: return "Pgdown";
		case VK_END: return "End";
		case VK_HOME: return "Home";
		case VK_LEFT: return "Left";
		case VK_UP: return "Up";
		case VK_RIGHT: return "Right";
		case VK_DOWN: return "Down";
		case VK_SNAPSHOT: return "Print screen";
		case VK_INSERT: return "Insert";
		case VK_DELETE: return "Delete";
		case VK_HELP: return "Help";



		case VK_ABNT_C1			: return "Abnt C1";
		case VK_ABNT_C2			: return "Abnt C2";
		case VK_ADD				: return "Numpad +";
		case VK_ATTN			: return "Attn";
		case VK_BACK			: return "Backspace";
		case VK_CANCEL			: return "Break";
		case VK_CLEAR			: return "Clear";
		case VK_CRSEL			: return "Cr Sel";
		case VK_DECIMAL			: return "Numpad .";
		case VK_DIVIDE			: return "Numpad /";
		case VK_EREOF			: return "Er Eof";
		case VK_ESCAPE			: return "Esc";
		case VK_EXECUTE			: return "Execute";
		case VK_EXSEL			: return "Ex Sel";
		case VK_ICO_CLEAR		: return "IcoClr";
		case VK_ICO_HELP		: return "IcoHlp";
		case VK_KEY_0			: return "0";
		case VK_KEY_1			: return "1";
		case VK_KEY_2			: return "2";
		case VK_KEY_3			: return "3";
		case VK_KEY_4			: return "4";
		case VK_KEY_5			: return "5";
		case VK_KEY_6			: return "6";
		case VK_KEY_7			: return "7";
		case VK_KEY_8			: return "8";
		case VK_KEY_9			: return "9";
		case VK_KEY_A			: return "A";
		case VK_KEY_B			: return "B";
		case VK_KEY_C			: return "C";
		case VK_KEY_D			: return "D";
		case VK_KEY_E			: return "E";
		case VK_KEY_F			: return "F";
		case VK_KEY_G			: return "G";
		case VK_KEY_H			: return "H";
		case VK_KEY_I			: return "I";
		case VK_KEY_J			: return "J";
		case VK_KEY_K			: return "K";
		case VK_KEY_L			: return "L";
		case VK_KEY_M			: return "M";
		case VK_KEY_N			: return "N";
		case VK_KEY_O			: return "O";
		case VK_KEY_P			: return "P";
		case VK_KEY_Q			: return "Q";
		case VK_KEY_R			: return "R";
		case VK_KEY_S			: return "S";
		case VK_KEY_T			: return "T";
		case VK_KEY_U			: return "U";
		case VK_KEY_V			: return "V";
		case VK_KEY_W			: return "W";
		case VK_KEY_X			: return "X";
		case VK_KEY_Y			: return "Y";
		case VK_KEY_Z			: return "Z";
		case VK_MULTIPLY		: return "Numpad *";
		case VK_NONAME			: return "NoName";
		case VK_NUMPAD0			: return "Numpad 0";
		case VK_NUMPAD1			: return "Numpad 1";
		case VK_NUMPAD2			: return "Numpad 2";
		case VK_NUMPAD3			: return "Numpad 3";
		case VK_NUMPAD4			: return "Numpad 4";
		case VK_NUMPAD5			: return "Numpad 5";
		case VK_NUMPAD6			: return "Numpad 6";
		case VK_NUMPAD7			: return "Numpad 7";
		case VK_NUMPAD8			: return "Numpad 8";
		case VK_NUMPAD9			: return "Numpad 9";
		case VK_OEM_1			: return "OEM_1(:;)";
		case VK_OEM_102			: return "OEM_102(> < )";
		case VK_OEM_2			: return "OEM_2(? / )";
		case VK_OEM_3			: return "OEM_3(~`)";
		case VK_OEM_4			: return "OEM_4({ [)";
		case VK_OEM_5			: return "OEM_5(| )";
		case VK_OEM_6			: return "OEM_6(}] )";
		case VK_OEM_7			: return "OEM_7(\" ')";
		case VK_OEM_8			: return "OEM_8(§ !)";
		case VK_OEM_ATTN		: return "Oem Attn";
		case VK_OEM_AUTO		: return "Auto";
		case VK_OEM_AX			: return "Ax";
		case VK_OEM_BACKTAB		: return "Back Tab";
		case VK_OEM_CLEAR		: return "OemClr";
		case VK_OEM_COMMA		: return "OEM_COMMA(< , )";
		case VK_OEM_COPY		: return "Copy";
		case VK_OEM_CUSEL		: return "Cu Sel";
		case VK_OEM_ENLW		: return "Enlw";
		case VK_OEM_FINISH		: return "Finish";
		case VK_OEM_FJ_LOYA		: return "Loya";
		case VK_OEM_FJ_MASSHOU	: return "Mashu";
		case VK_OEM_FJ_ROYA		: return "Roya";
		case VK_OEM_FJ_TOUROKU	: return "Touroku";
		case VK_OEM_JUMP		: return "Jump";
		case VK_OEM_MINUS		: return "OEM_MINUS(_ - )";
		case VK_OEM_PA1			: return "OemPa1";
		case VK_OEM_PA2			: return "OemPa2";
		case VK_OEM_PA3			: return "OemPa3";
		case VK_OEM_PERIOD		: return "OEM_PERIOD(> .)";
		case VK_OEM_PLUS		: return "OEM_PLUS(+= )";
		case VK_OEM_RESET		: return "Reset";
		case VK_OEM_WSCTRL		: return "WsCtrl";
		case VK_PA1				: return "Pa1";
		case VK_PACKET			: return "Packet";
		case VK_PLAY			: return "Play";
		case VK_PROCESSKEY		: return "Process";
		case VK_RETURN			: return "Enter";
		case VK_SELECT			: return "Select";
		case VK_SEPARATOR		: return "Separator";
		case VK_SPACE			: return "Space";
		case VK_SUBTRACT		: return "Num -";
		case VK_TAB				: return "Tab";
		case VK_ZOOM			: return "Zoom";
		default					: return "N/A";
		}
	}

}
