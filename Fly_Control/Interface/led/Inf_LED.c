#include "Inf_LED.h"

void Inf_LED_SetStatus(LedStruct *led)
{
    if(led->status<=1)
    {
        if(led->location==LEFT_TOP)
        {
					HAL_GPIO_WritePin(LED_LEFT_TOP_GPIO_Port,
														LED_LEFT_TOP_Pin,
														(GPIO_PinState)led->status);
        }
				else if(led->location==LEFT_BOTTOM)
        {
					HAL_GPIO_WritePin(LED_LEFT_BOTTOM_GPIO_Port,
														LED_LEFT_BOTTOM_Pin,
														(GPIO_PinState)led->status);
        }
				else if(led->location==RIGHT_TOP)
        {
					HAL_GPIO_WritePin(LED_RIGHT_TOP_GPIO_Port,
														LED_RIGHT_TOP_Pin,
														(GPIO_PinState)led->status);
        }
				else if(led->location==RIGHT_BOTTOM)
        {
					HAL_GPIO_WritePin(LED_RIGHT_BOTTOM_GPIO_Port,
														LED_RIGHT_BOTTOM_Pin,
														(GPIO_PinState)led->status);
        }
    }
		else
		{
			/*这里不控制闪烁周期，只对led状态做翻转*/
        if(led->location==LEFT_TOP)
        {
					HAL_GPIO_TogglePin(LED_LEFT_TOP_GPIO_Port,
														LED_LEFT_TOP_Pin);
        }
				else if(led->location==LEFT_BOTTOM)
        {
					HAL_GPIO_TogglePin(LED_LEFT_BOTTOM_GPIO_Port,
														LED_LEFT_BOTTOM_Pin);
        }
				else if(led->location==RIGHT_TOP)
        {
					HAL_GPIO_TogglePin(LED_RIGHT_TOP_GPIO_Port,
														LED_RIGHT_TOP_Pin);
        }
				else if(led->location==RIGHT_BOTTOM)
        {
					HAL_GPIO_TogglePin(LED_RIGHT_BOTTOM_GPIO_Port,
														LED_RIGHT_BOTTOM_Pin);
        }		
		}
}
