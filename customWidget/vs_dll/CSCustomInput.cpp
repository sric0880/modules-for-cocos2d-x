#include "CSCustomInput.h"

namespace CocoStudio{
	namespace EngineAdapter{

		CSCustomInput::CSCustomInput(void):scale9Sprite(nullptr)
		{
			m_GUI = CustomInput::create();
			m_GUI->retain();
			this->Init();
		}


		CSCustomInput::~CSCustomInput(void)
		{
		}

		CustomInput* CSCustomInput::GetInnerObject()
		{
			return dynamic_cast<CustomInput*>(m_GUI);
		}

		 void CSCustomInput::InitPropertyEx()
		 {
			 CSWidget::InitPropertyEx();
			 bInitWithDefaultIcon = true;
			 normalFile = CSResourceData("");
			 this->flag = 4;
			 this->mode = 6;
			 this->fontSize=30;
			 this->size = "{100,80}";
			 this->rect = "{{0,0},{0,0}}";
		 }
		void CSCustomInput::AfterAdded()
        {
			CS_SetFontName("Arial");
			CS_SetFontSize(this->fontSize);
			CS_SetInputFlag(this->flag);
			CS_SetInputMode(this->mode);
			CS_SetMaxLength(0);
			CS_SetPlaceHolder("Test");
			CS_SetCustomSize(this->size.c_str());
			CS_SetScale9Rect(this->rect.c_str());
        }

		void CSCustomInput::CS_SetScale9Rect(const char* rect)
		{
			this->rect = rect;
			CCRect r = CCRectFromString(rect);
			if(scale9Sprite){
				scale9Sprite->setCapInsets(r);
			}
		}
		void CSCustomInput::CS_SetCustomSize(const char* size)
		{
			this->size = size;
			CCSize s = CCSizeFromString(size);
			if(scale9Sprite)
				GetInnerObject()->initWithSizeAndBackgroundSprite(s,scale9Sprite);
		}
		void CSCustomInput::CS_SetSpriteFile(CSResourceData file)
		{
			std::string filePath = file.GetPath();
            if (filePath != "" && filePath.find(".png") != std::string::npos)
            {
				normalFile = file;
				scale9Sprite =  CCScale9Sprite::createWithSpriteFrameName(file.GetPath().c_str());
				GetInnerObject()->initWithSizeAndBackgroundSprite(CCSizeFromString(size.c_str()),scale9Sprite);
            }
		}
		void CSCustomInput::CS_SetFontName(const char* pFontName)
		{
			this->fontName = pFontName;
			GetInnerObject()->setFontName(std::move(fontName));
		}
		void CSCustomInput::CS_SetFontSize(int fontSize)
		{
			this->fontSize = fontSize;
			GetInnerObject()->setFontSize(fontSize);
		}

		void CSCustomInput::CS_SetPlaceHolder(const char* text)
		{
			this->placeHolder = text;
			GetInnerObject()->setPlaceHolder(text);
		}
		void CSCustomInput::CS_SetMaxLength(int maxlen)
		{
			GetInnerObject()->setMaxLength(maxlen);
		}
		void CSCustomInput::CS_SetInputFlag(int flag)
		{
			this->flag = flag;
			GetInnerObject()->setInputFlag((EditBoxInputFlag)flag);
		}
		void CSCustomInput::CS_SetInputMode(int mode)
		{
			this->mode = mode;
			GetInnerObject()->setInputMode((EditBoxInputMode)mode);
		}

		const char* CSCustomInput::CS_GetScale9Rect()
		{
			return rect.c_str();
		}
		const char* CSCustomInput::CS_GetCustomSize()
		{
			return size.c_str();
		}
		CSResourceData  CSCustomInput::CS_GetSpriteFile()
		{
			return normalFile;
		}
		const  char* CSCustomInput::CS_GetFontName()
		{
			return this->fontName.c_str();
		}
		const char*  CSCustomInput::CS_GetPlaceHolder()
		{
			return this->placeHolder.c_str();
		}
		int  CSCustomInput::CS_GetFontSize()
		{
			return this->fontSize;
		}
		int  CSCustomInput::CS_GetMaxLength()
		{
			return GetInnerObject()->getMaxLength();
		}
		int  CSCustomInput::CS_GetInputFlag()
		{
			return this->flag;
		}
		int  CSCustomInput::CS_GetInputMode()
		{
			return this->mode;
		}

		void CSCustomInput::CloneWidgetCustomProperty(CSWidget* cWidget)
        {
            CSCustomInput* csWidget = dynamic_cast<CSCustomInput*>(cWidget);
            if (csWidget != NULL)
            {
				this->CS_SetScale9Rect(csWidget->CS_GetScale9Rect());
				this->CS_SetSpriteFile(csWidget->CS_GetSpriteFile());
				this->CS_SetFontName(csWidget->CS_GetFontName());
				this->CS_SetFontSize(csWidget->CS_GetFontSize());
				this->CS_SetPlaceHolder(csWidget->CS_GetPlaceHolder());
				this->CS_SetMaxLength(csWidget->CS_GetMaxLength());
				this->CS_SetInputFlag(csWidget->CS_GetInputFlag());
				this->CS_SetInputMode(csWidget->CS_GetInputMode());
            }
        }
	}
}
