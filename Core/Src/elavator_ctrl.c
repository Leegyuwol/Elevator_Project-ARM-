#include "elavator_ctrl.h"
#include "stepper.h"


int currentfloor = 0;

extern uint8_t is_motor_working;
extern uint8_t direction;	// DIR_CW(high), DIR_CCW (low)


uint8_t check_1f;
uint8_t check_2f;
uint8_t check_3f;







void updateCurrentFloor() {
    if (HAL_GPIO_ReadPin(photoint_1f_GPIO_Port, photoint_1f_Pin)) {
        currentfloor = 1; // 현재 1층
    } else if (HAL_GPIO_ReadPin(photoint_2f_GPIO_Port, photoint_2f_Pin)) {
        currentfloor = 2; // 현재 2층
    } else if (HAL_GPIO_ReadPin(photoint_3f_GPIO_Port, photoint_3f_Pin)) {
        currentfloor = 3; // 현재 3층
    }
    FND_DisplayNumber(currentfloor);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{

	updateCurrentFloor();

	if (GPIO_Pin == button_1f_Pin) check_1f = 1;
	if (GPIO_Pin == button_2f_Pin) check_2f = 1;
	if (GPIO_Pin == button_3f_Pin) check_3f = 1;


	if (currentfloor == 1) check_1f = 0;
	if (currentfloor == 2) check_2f = 0;
	if (currentfloor == 3) check_3f = 0;


	// motor working?
	if (check_1f || check_2f || check_3f)
		is_motor_working = 1;
	else
		is_motor_working = 0;

	// motor direction
	switch (currentfloor)
	{
	case 1:
		if (check_2f || check_3f) direction = DIR_CW;
		break;
	case 2:
		{
			if (check_1f && !check_3f) direction = DIR_CCW;
			if (check_3f && !check_1f) direction = DIR_CW;
		}
		break;
	case 3:
		if (check_1f || check_2f) direction = DIR_CCW;
		break;
	}
}

/*
	switch (currentfloor)
	{
	case 1:

		// 1번 버튼 (이미 1층이니 동작x)
		if(check_1f)
		    {
				is_motor_working = 0;		// 모터 정지 및 알림
				updateCurrentFloor();			// 현재 층수 파악
		    }

		// 2번 버튼
		if(check_2f)
		    {
				direction = DIR_CW;	// 1 -> 2 상승
				is_motor_working = 1;

			// 상승 중 포토인터럽트 2번 검출 시
			if (HAL_GPIO_ReadPin(photoint_2f_GPIO_Port, photoint_2f_Pin))
					{
						is_motor_working = 0;		// 모터 정지
						updateCurrentFloor();			// 현재 층수 파악
					}
		    }

		// 3번 버튼
		if(check_3f)
		    {
				direction = DIR_CW;	// 1 -> 3 상승
				is_motor_working = 1;

			// 상승 중 포토인터럽트 3번 검출 시
			if (HAL_GPIO_ReadPin(photoint_3f_GPIO_Port, photoint_3f_Pin))
					{
						is_motor_working = 0;		// 모터 정지
						updateCurrentFloor();			// 현재 층수 파악
					}
			// 1 -> 3 층 올라갈때 2층 포토인터럽트 검출시 2층 fnd 출력
			else if (HAL_GPIO_ReadPin(photoint_2f_GPIO_Port, photoint_2f_Pin))
				{
					updateCurrentFloor();
				}

		    }

		break;

	case 2:

		// 2번 버튼 (이미 2층이니 동작x)
		if(check_2f)
		    {
				is_motor_working = 0;		// 모터 정지 및 알림
				updateCurrentFloor();			// 현재 층수 파악
		    }

		// 1번 버튼
		if(check_1f)
		    {
				direction = DIR_CCW;	// 2 -> 1 하강
				is_motor_working = 1;

			// 하강 중 포토인터럽트 1번 검출 시
			if (HAL_GPIO_ReadPin(photoint_1f_GPIO_Port, photoint_1f_Pin))
					{
						is_motor_working = 0;		// 모터 정지
						updateCurrentFloor();			// 현재 층수 파악
					}
		    }

		// 3번 버튼
		if(check_3f)
		    {
				direction = DIR_CW;	// 2 -> 3 상승
				is_motor_working = 1;

			// 상승 중 포토인터럽트 3번 검출 시
			if (HAL_GPIO_ReadPin(photoint_3f_GPIO_Port, photoint_3f_Pin))
					{
						is_motor_working = 0;		// 모터 정지
						updateCurrentFloor();			// 현재 층수 파악
					}
		    }

		break;


	case 3:

		// 3번 버튼 (이미 3층이니 동작x)
		if(check_3f)
		    {
				is_motor_working = 0;		// 모터 정지 및 알림
				updateCurrentFloor();			// 현재 층수 파악
		    }


		// 2번 버튼
		if(check_2f)
		    {
				direction = DIR_CCW;	// 3 -> 2 하강
				is_motor_working = 1;

			// 하강 중 포토인터럽트 2번 검출 시
			if (HAL_GPIO_ReadPin(photoint_2f_GPIO_Port, photoint_2f_Pin))
					{
						is_motor_working = 0;		// 모터 정지
						updateCurrentFloor();			// 현재 층수 파악
					}
		    }

		// 1번 버튼
		if(check_1f)
		    {
				direction = DIR_CCW;	// 3 -> 1 하강
				is_motor_working = 1;

			// 하강 중 포토인터럽트 1번 검출 시
			if (HAL_GPIO_ReadPin(photoint_1f_GPIO_Port, photoint_1f_Pin))
					{
						is_motor_working = 0;			// 모터 정지
						updateCurrentFloor();			// 현재 층수 파악
					}
			else if (HAL_GPIO_ReadPin(photoint_2f_GPIO_Port, photoint_2f_Pin))
				{
					updateCurrentFloor();
				}
		    }


		break;

		default :


				if (HAL_GPIO_ReadPin(photoint_1f_GPIO_Port, photoint_1f_Pin))
						{
								is_motor_working = 0;
								updateCurrentFloor();
						}

				else if (HAL_GPIO_ReadPin(photoint_2f_GPIO_Port, photoint_2f_Pin))
						{
								is_motor_working = 0;
								updateCurrentFloor();
						}

				else if (HAL_GPIO_ReadPin(photoint_3f_GPIO_Port, photoint_3f_Pin))
						{
								is_motor_working = 0;
								updateCurrentFloor();
						}


		}

	if (HAL_GPIO_ReadPin(photoint_1f_GPIO_Port, photoint_1f_Pin))
	{
		if (currentfloor == 1)
		{
			check_1f = 0;
		}
	}
	else if(GPIO_Pin == button_1f_Pin)
	{
		check_1f = 1;
	}

	if (HAL_GPIO_ReadPin(photoint_2f_GPIO_Port, photoint_2f_Pin))
	{
		if (currentfloor == 2)
		{
			check_2f = 0;
		}
	}
	else if(GPIO_Pin == button_2f_Pin)
	{
		check_2f = 1;
	}

	if (HAL_GPIO_ReadPin(photoint_3f_GPIO_Port, photoint_3f_Pin))
	{
		if (currentfloor == 3)
		{
			check_3f = 0;
		}
	}
	else if(GPIO_Pin == button_3f_Pin)
	{
		check_3f = 1;
	}

}*/


