#include "../Inc/key.h"
#include "stdbool.h"

static bool any_key_press = false;
static Key_State get_key_press = no_key_press;

extern bool check_key_press(void) {
	if (any_key_press) {
		any_key_press = false;
		get_key_press = no_key_press;
		return true;
	}
	return false;
}

extern Key_State check_key(void) {
	if(get_key_press != no_key_press) {
		// 此处需要关中断保证原子性吗？
		Key_State temp = get_key_press;
		get_key_press = no_key_press;
		return temp;
	}
	return no_key_press;
}

void Key_Init(void) {
	GPIO_InitTypeDef GPIO_InitStructure;
	
	KEY0_GPIO_CLK_ENABLE();
	KEY1_GPIO_CLK_ENABLE();
	KEY2_GPIO_CLK_ENABLE();
	KEY3_GPIO_CLK_ENABLE();
	
	GPIO_InitStructure.Mode = GPIO_MODE_IT_RISING;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	
	GPIO_InitStructure.Pin = KEY0_GPIO_PIN;
	HAL_GPIO_Init(KEY0_GPIO_PORT, &GPIO_InitStructure);
	HAL_NVIC_SetPriority(KEY0_EXIT_IRQ,0,0);
	HAL_NVIC_EnableIRQ(KEY0_EXIT_IRQ);
	
	GPIO_InitStructure.Pin = KEY1_GPIO_PIN;
	HAL_GPIO_Init(KEY1_GPIO_PORT, &GPIO_InitStructure);
	HAL_NVIC_SetPriority(KEY1_EXIT_IRQ,0,0);
	HAL_NVIC_EnableIRQ(KEY1_EXIT_IRQ);
	
	GPIO_InitStructure.Pin = KEY2_GPIO_PIN;
	HAL_GPIO_Init(KEY2_GPIO_PORT, &GPIO_InitStructure);
	HAL_NVIC_SetPriority(KEY2_EXIT_IRQ,0,0);
	HAL_NVIC_EnableIRQ(KEY2_EXIT_IRQ);
	
	GPIO_InitStructure.Pin = KEY3_GPIO_PIN;
	HAL_GPIO_Init(KEY3_GPIO_PORT, &GPIO_InitStructure);
	HAL_NVIC_SetPriority(KEY3_EXIT_IRQ,0,0);
	HAL_NVIC_EnableIRQ(KEY3_EXIT_IRQ);
}

void KEY0_IRQHandler (void) {
	any_key_press = true;
	if (__HAL_GPIO_EXTI_GET_IT(KEY0_GPIO_PIN) != RESET) {
    __HAL_GPIO_EXTI_CLEAR_IT(KEY0_GPIO_PIN);
		get_key_press = key_0;
  }
	
	if (__HAL_GPIO_EXTI_GET_IT(KEY3_GPIO_PIN) != RESET) {
    __HAL_GPIO_EXTI_CLEAR_IT(KEY3_GPIO_PIN);
		get_key_press = key_3;
  }
}

void KEY1_IRQHandler (void) {
	any_key_press = true;
	if (__HAL_GPIO_EXTI_GET_IT(KEY1_GPIO_PIN) != RESET) {
    __HAL_GPIO_EXTI_CLEAR_IT(KEY1_GPIO_PIN);
		get_key_press = key_1;
  }
	
	if (__HAL_GPIO_EXTI_GET_IT(KEY2_GPIO_PIN) != RESET) {
    __HAL_GPIO_EXTI_CLEAR_IT(KEY2_GPIO_PIN);
		get_key_press = key_2;
  }
}