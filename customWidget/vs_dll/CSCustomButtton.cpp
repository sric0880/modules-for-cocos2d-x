#include "CSCustomButton.h"

namespace CocoStudio{
	namespace EngineAdapter{

		CSCustomButton::CSCustomButton()
		{
			m_GUI = CustomButton::create();
			m_GUI->retain();
			this->Init();
		}

		CSCustomButton::~CSCustomButton()
		{
		}

		/*void CSCustomLabel::InitPropertyEx()
		{
			CSWidget::InitPropertyEx();
		}*/

		void CSCustomButton::AfterAdded()
        {
        }

		CustomButton* CSCustomButton::GetInnerObject()
		{
			return dynamic_cast<CustomButton*>(m_GUI);
		}
        
		void CSCustomButton::CS_SetTextId(const char* textid)
		{
			this->textId = textid;
		}
		const char* CSCustomButton::CS_GetTextId()
		{
			return this->textId.c_str();
		}
		void CSCustomButton::CS_SetTTFConfigId(const char* id)
		{
			this->ttfConfig = id;
		}
		const char* CSCustomButton::CS_GetTTFConfigId()
		{
			return this->ttfConfig.c_str();
		}
		void CSCustomButton::CS_SetBMFConfigId(const char* id)
		{
			this->bmfConfig = id;
		}
		const char* CSCustomButton::CS_GetBMFConfigId()
		{
			return this->bmfConfig.c_str();
		}

		void CSCustomButton::CloneWidgetCustomProperty(CSWidget* cWidget)
        {
            CSCustomButton* csWidget = dynamic_cast<CSCustomButton*>(cWidget);
            if (csWidget != NULL)
            {
				this->CS_SetTextId(csWidget->CS_GetTextId());
				this->CS_SetTTFConfigId(csWidget->CS_GetTTFConfigId());
				this->CS_SetBMFConfigId(csWidget->CS_GetBMFConfigId());
            }
        }
	}
}
