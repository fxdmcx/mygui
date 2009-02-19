﻿/*!
	@file
	@author		Generate utility by Albert Semenov
	@date		01/2009
	@module
*/

#include "../ExportDefine.h"
#include "../ExportMarshaling.h"
#include <MyGUI.h>

namespace Export
{

	//InsertPoint

   	namespace
	{
		MYGUIEXPORT void MYGUICALL ExportMessage_SetMessageStyle_style( MyGUI::Widget* _native ,
			Convert< MyGUI::MessageBoxStyle >::Type _style )
		{
			static_cast< MyGUI::Message * >(_native)->setMessageStyle(
				Convert< MyGUI::MessageBoxStyle >::From(_style) );
		}
	}



   	namespace
	{
		MYGUIEXPORT void MYGUICALL ExportMessage_SetMessageButton_button( MyGUI::Widget* _native ,
			Convert< MyGUI::MessageBoxStyle >::Type _button )
		{
			static_cast< MyGUI::Message * >(_native)->setMessageButton(
				Convert< MyGUI::MessageBoxStyle >::From(_button) );
		}
	}



   	namespace
	{
		MYGUIEXPORT void MYGUICALL ExportMessage_EndMessage( MyGUI::Widget* _native )
		{
			static_cast< MyGUI::Message * >(_native)->endMessage( );
		}
	}



   	namespace
	{
		MYGUIEXPORT void MYGUICALL ExportMessage_EndMessage_result( MyGUI::Widget* _native ,
			Convert< MyGUI::MessageBoxStyle >::Type _result )
		{
			static_cast< MyGUI::Message * >(_native)->endMessage(
				Convert< MyGUI::MessageBoxStyle >::From(_result) );
		}
	}



   	namespace
	{
		MYGUIEXPORT void MYGUICALL ExportMessage_SetWindowFade_fade( MyGUI::Widget* _native ,
			Convert<bool>::Type _fade )
		{
			static_cast< MyGUI::Message * >(_native)->setWindowFade(
				Convert<bool>::From(_fade) );
		}
	}



   	namespace
	{
		MYGUIEXPORT void MYGUICALL ExportMessage_SetMessageIcon_icon( MyGUI::Widget* _native ,
			Convert< MyGUI::MessageBoxStyle >::Type _icon )
		{
			static_cast< MyGUI::Message * >(_native)->setMessageIcon(
				Convert< MyGUI::MessageBoxStyle >::From(_icon) );
		}
	}



   	namespace
	{
		MYGUIEXPORT Convert<const std::string &>::Type MYGUICALL ExportMessage_GetDefaultLayer( MyGUI::Widget* _native )
		{
			return Convert<const std::string &>::To( static_cast< MyGUI::Message * >(_native)->getDefaultLayer( ) );
		}
	}



   	namespace
	{
		MYGUIEXPORT void MYGUICALL ExportMessage_SetSmoothShow_smooth( MyGUI::Widget* _native ,
			Convert<bool>::Type _smooth )
		{
			static_cast< MyGUI::Message * >(_native)->setSmoothShow(
				Convert<bool>::From(_smooth) );
		}
	}



   	namespace
	{
		MYGUIEXPORT Convert< MyGUI::MessageBoxStyle >::Type MYGUICALL ExportMessage_AddButtonName_name( MyGUI::Widget* _native ,
			Convert<const Ogre::UTFString &>::Type _name )
		{
			return Convert< MyGUI::MessageBoxStyle >::To( static_cast< MyGUI::Message * >(_native)->addButtonName(
				Convert<const Ogre::UTFString &>::From(_name) ) );
		}
	}



   	namespace
	{
		MYGUIEXPORT void MYGUICALL ExportMessage_SetMessageText_message( MyGUI::Widget* _native ,
			Convert<const Ogre::UTFString &>::Type _message )
		{
			static_cast< MyGUI::Message * >(_native)->setMessageText(
				Convert<const Ogre::UTFString &>::From(_message) );
		}
	}



}
