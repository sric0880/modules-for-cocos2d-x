#include "CSCustomLabel.h"

namespace CocoStudio{
	namespace EngineAdapter{

		CSCustomLabel::CSCustomLabel()
		{
			m_GUI = CustomLabel::create();
			m_GUI->retain();
			this->Init();
		}

		CSCustomLabel::~CSCustomLabel()
		{
		}

		void CSCustomLabel::InitPropertyEx()
		{
			CSWidget::InitPropertyEx();
			bInitWithDefaultIcon = true;
		}

		void CSCustomLabel::AfterAdded()
        {
        }

		CustomLabel* CSCustomLabel::GetInnerObject()
		{
			return dynamic_cast<CustomLabel*>(m_GUI);
		}
        
		void CSCustomLabel::CS_SetTextId(const char* textid)
		{
			this->textId = textid;
		}
		const char* CSCustomLabel::CS_GetTextId()
		{
			return this->textId.c_str();
		}
		void CSCustomLabel::CS_SetTTFConfigId(const char* id)
		{
			this->ttfConfig = id;
		}
		const char* CSCustomLabel::CS_GetTTFConfigId()
		{
			return this->ttfConfig.c_str();
		}
		void CSCustomLabel::CS_SetBMFConfigId(const char* id)
		{
			this->bmfConfig = id;
		}
		const char* CSCustomLabel::CS_GetBMFConfigId()
		{
			return this->bmfConfig.c_str();
		}

		void CSCustomLabel::CloneWidgetCustomProperty(CSWidget* cWidget)
        {
            CSCustomLabel* csWidget = dynamic_cast<CSCustomLabel*>(cWidget);
            if (csWidget != NULL)
            {
				this->CS_SetTextId(csWidget->CS_GetTextId());
				this->CS_SetTTFConfigId(csWidget->CS_GetTTFConfigId());
				this->CS_SetBMFConfigId(csWidget->CS_GetBMFConfigId());
            }
        }
	}
}
