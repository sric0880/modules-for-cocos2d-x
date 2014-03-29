#pragma once
#include "CSWidget.h"
#include "CustomInput.h"
using namespace cocos2d;

namespace CocoStudio{
	namespace EngineAdapter{

		class CSCustomInput :
			public CSWidget
		{
		private:
			CustomInput* GetInnerObject();
			std::string rect;
			std::string size;
			CCScale9Sprite* scale9Sprite;
			CSResourceData normalFile;
			std::string fontName;
			std::string placeHolder;
			int fontSize;
			int flag;
			int mode;
		public:
			CSCustomInput(void);
			~CSCustomInput(void);
			virtual void InitPropertyEx()override;
			virtual void AfterAdded()override;

			void CS_SetScale9Rect(const char* rect);
			void CS_SetCustomSize(const char* size);
			void CS_SetSpriteFile(CSResourceData normalFile);
			void CS_SetFontName(const char* pFontName);
			void CS_SetFontSize(int fontSize);
			void CS_SetPlaceHolder(const char* text);
			void CS_SetMaxLength(int maxlen);
			void CS_SetInputFlag(int);
			void CS_SetInputMode(int);

			const char* CS_GetScale9Rect();
			const char* CS_GetCustomSize();
			CSResourceData CS_GetSpriteFile();
			const char* CS_GetFontName();
			const char* CS_GetPlaceHolder();
			int CS_GetFontSize();
			int CS_GetMaxLength();
			int CS_GetInputFlag();
			int CS_GetInputMode();

			void CloneWidgetCustomProperty(CSWidget* cWidget) override;
		};

	}
}

