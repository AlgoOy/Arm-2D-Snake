#include <stdlib.h>
#include <time.h>
#include "../Inc/snake.h"
#include "key.h"

static Game_State_Info gameState = {0};
static Snake *snake_head = NULL;
static Direction snake_direction = Right;

static uint16_t map_width = HORIZONTAL_NUM_MAX;
static uint16_t map_height = VERTICAL_NUM_MAX;

extern const arm_2d_tile_t c_tileSnakeLogoRGB565;
extern const arm_2d_tile_t c_tileSnakeLogoMask;

extern const arm_2d_tile_t c_tileFruitRGB565;
extern const arm_2d_tile_t c_tileFruitMask;

extern const arm_2d_tile_t c_tileSnakeBodyRGB565;
extern const arm_2d_tile_t c_tileSnakeBodyMask;

Game_State_Info getGameState(void) {
	return gameState;
}

SnakeGameStatus DrawStartGamePanel(const arm_2d_tile_t *ptTile, DrawSenceSelection ground) {
	if(ground == background) {
		/* background */
		arm_2d_canvas(ptTile, __top_canvas) {
			arm_2d_fill_colour(ptTile, NULL, GLCD_COLOR_WHITE);
			
			arm_2d_align_top_centre(__top_canvas, c_tileSnakeLogoRGB565.tRegion.tSize) {
				arm_2d_rgb565_tile_copy_with_src_mask(
					&c_tileSnakeLogoRGB565,
					&c_tileSnakeLogoMask,
					ptTile,
					&__top_centre_region,
					ARM_2D_CP_MODE_COPY
				);
			}
		}
		/* background */
	} else {
		/* foreground */
		arm_2d_canvas(ptTile, __top_canvas) {
			const char* pchString = "Press Any Key to Start Game";
			
			int16_t textWidth = strlen(pchString) * ARM_2D_FONT_6x8.use_as__arm_2d_font_t.tCharSize.iWidth;
			
			arm_lcd_text_set_target_framebuffer((arm_2d_tile_t *)ptTile);
			arm_lcd_text_set_font(&ARM_2D_FONT_6x8.use_as__arm_2d_font_t);
			arm_lcd_text_set_draw_region(NULL);
			
			arm_2dp_fill_colour_with_opacity(
                    NULL, 
                    ptTile, 
                    (arm_2d_region_t []){
                        {
                            .tLocation = {
                                .iX = 2,
                                .iY = __GLCD_CFG_SCEEN_HEIGHT__ - 17},
                            .tSize = {
                                .iWidth = __GLCD_CFG_SCEEN_WIDTH__ - 4,
                                .iHeight = 9,
                            },
                        },
                    }, 
                    (__arm_2d_color_t){__RGB(64, 64, 64)},
                    255 - 128);
			arm_2d_op_wait_async(NULL);
			arm_lcd_text_set_colour(GLCD_COLOR_GREEN, GLCD_COLOR_WHITE);
			arm_lcd_text_location((__GLCD_CFG_SCEEN_HEIGHT__ + 7) / 8 - 2,
										((__GLCD_CFG_SCEEN_WIDTH__ / ARM_2D_FONT_6x8.use_as__arm_2d_font_t.tCharSize.iWidth) - strlen(pchString)) / 2);
			arm_lcd_printf("%s", pchString);
    }
		/* foreground */
	}
	return Snake_Game_No_Error;
}

SnakeGameStatus DrawRunningGamePanel(const arm_2d_tile_t *ptTile, DrawSenceSelection ground) {
	if (ground == background) {
		/* background */
		arm_2d_canvas(ptTile, __top_canvas) {
			/* bottom */
			arm_2dp_fill_colour_with_opacity(
										NULL, 
										ptTile, 
										(arm_2d_region_t []){
											{
												.tLocation = {
													.iX = 0,
													.iY = STATE_VIEW_HEIGHT + HEIGHT_PIXELS_USED_BY_GAME,
												},
												.tSize = {
													.iWidth = __GLCD_CFG_SCEEN_WIDTH__,
													.iHeight = __GLCD_CFG_SCEEN_HEIGHT__ - (STATE_VIEW_HEIGHT + HEIGHT_PIXELS_USED_BY_GAME),
												},
											},
										}, 
										(__arm_2d_color_t){__RGB(64, 64, 64)},
										255 - 128
									);
			/* left */
			arm_2dp_fill_colour_with_opacity(
										NULL, 
										ptTile, 
										(arm_2d_region_t []){
											{
												.tLocation = {
													.iX = 0,
													.iY = STATE_VIEW_HEIGHT,
												},
												.tSize = {
													.iWidth = (__GLCD_CFG_SCEEN_WIDTH__ - WIDTH_PIXELS_USED_BY_GAME) / 2,
													.iHeight = HEIGHT_PIXELS_USED_BY_GAME,
												},
											},
										}, 
										(__arm_2d_color_t){__RGB(64, 64, 64)},
										255 - 128
									);
			/* right */
			arm_2dp_fill_colour_with_opacity(
										NULL, 
										ptTile, 
										(arm_2d_region_t []){
											{
												.tLocation = {
													.iX = __GLCD_CFG_SCEEN_WIDTH__ - ((__GLCD_CFG_SCEEN_WIDTH__ - WIDTH_PIXELS_USED_BY_GAME) / 2),
													.iY = STATE_VIEW_HEIGHT,
												},
												.tSize = {
													.iWidth = (__GLCD_CFG_SCEEN_WIDTH__ - WIDTH_PIXELS_USED_BY_GAME) / 2,
													.iHeight = HEIGHT_PIXELS_USED_BY_GAME,
												},
											},
										}, 
										(__arm_2d_color_t){__RGB(64, 64, 64)},
										255 - 128
									);
		}
		/* background */
	}	else {
		/* foreground */
		arm_2d_canvas(ptTile, __top_canvas) {			
			Game_State_Info gameState = getGameState();
			
			arm_2d_fill_colour(ptTile, NULL, GLCD_COLOR_WHITE);
			
			arm_2dp_fill_colour_with_opacity(
                    NULL, 
                    ptTile, 
                    (arm_2d_region_t []){
                        {
                            .tLocation = {
                                .iX = 0,
                                .iY = 0
														},
                            .tSize = {
                                .iWidth = __GLCD_CFG_SCEEN_WIDTH__,
                                .iHeight = STATE_VIEW_HEIGHT,
                            },
                        },
                    }, 
                    (__arm_2d_color_t){__RGB(64, 64, 64)},
                    255 - 128
									);
        arm_2d_op_wait_async(NULL);
			
			arm_lcd_text_set_target_framebuffer((arm_2d_tile_t *)ptTile);
			arm_lcd_text_set_font(&ARM_2D_FONT_6x8.use_as__arm_2d_font_t);
			arm_lcd_text_set_draw_region(NULL);
			
			arm_lcd_text_set_colour(GLCD_COLOR_GREEN, GLCD_COLOR_WHITE);
			arm_lcd_text_location(0, 2);
			arm_lcd_printf(" score: %4d\n  length: %4d", gameState.score, gameState.length);
										
			
    }
		/* foreground */
	}
	return Snake_Game_No_Error;
}

SnakeGameStatus DrawEndGamePanel(const arm_2d_tile_t *ptTile, DrawSenceSelection ground) {
	return Snake_Game_No_Error;
}
SnakeGameStatus DrawGameElements(const arm_2d_tile_t *ptTile) {
	Snake *draw_snake = snake_head;
	
	while(draw_snake != NULL) {
		const arm_2d_region_t body_loc = {
			.tLocation = {
				.iX = Cal_Loc_X(draw_snake->loc.x),
				.iY = Cal_Loc_Y(draw_snake->loc.y),
			},
			.tSize = {
				.iWidth = SNAKE_WIDTH_PIXELS,
				.iHeight = SNAKE_HEIGHT_PIXELS,
			},
		};
		arm_2d_rgb565_tile_copy_with_src_mask(
			&c_tileSnakeBodyRGB565,
			&c_tileSnakeBodyMask,
			ptTile,
			&body_loc,
			ARM_2D_CP_MODE_COPY
		);
		draw_snake = draw_snake->next;
	}
	return Snake_Game_No_Error;
}

SnakeGameStatus InitGame(void) {
	snake_head = (Snake *)malloc(sizeof(Snake));
	if (snake_head == NULL) {
		return Snake_Game_No_Memory;
	}
	
	do {
		snake_head->loc.x = 1;
		snake_head->loc.y = 1;
		snake_head->next = NULL;
	} while(0);
	
	snake_direction = Right;
	
	gameState.score = 0;
	gameState.length = 1;
	gameState.state = begin;
	gameState.speed = middle_speed;
	
	return Snake_Game_No_Error;
}

SnakeGameStatus CreateSnake(const arm_2d_tile_t *ptTile) {
	Snake *body = (Snake *)malloc(sizeof(Snake));
	if (body == NULL) {
		return Snake_Game_No_Memory;
	}
	return Snake_Game_No_Error;
}

SnakeGameStatus CreateFruit(const arm_2d_tile_t *ptTile) {
	srand(time(NULL));
	uint8_t snake_x = (uint8_t)rand() % HORIZONTAL_NUM_MAX, snake_y = (uint8_t)rand() % VERTICAL_NUM_MAX;
	
	
//	arm_2d_canvas(ptTile, __top_canvas) {
//			
//			arm_2d_align_centre(__top_canvas, c_tileFruitRGB565.tRegion.tSize) {
//				arm_2d_rgb565_tile_copy_with_src_mask(
//					&c_tileFruitRGB565,
//					&c_tileFruitMask,
//					ptTile,
//					&__centre_region,
//					ARM_2D_CP_MODE_COPY
//				);
//			}
//		}
	
	return Snake_Game_No_Error;
}

extern Key_State check_key(void);

static void set_move_direction(void){
	switch(check_key()) {
		case key_0:
			if (snake_direction != Left) {
				snake_direction = Right;
			}
			break;
		case key_1:
			if (snake_direction != Up) {
				snake_direction = Down;
			}
			break;
		case key_2:
			if (snake_direction != Right) {
				snake_direction = Left;
			}
			break;
		case key_3:
			if (snake_direction != Down) {
				snake_direction = Up;
			}
			break;
		case no_key_press:
		default:
			break;
	}
}

SnakeGameStatus GameLogic(void){
	Snake *move_snake = snake_head;
	
	Snake *snake_body = (Snake *)malloc(sizeof(Snake));
	if (snake_body == NULL) {
		return Snake_Game_No_Memory;
	}
	memcpy(snake_body, snake_head, sizeof(Snake));
	
	set_move_direction();
	switch(snake_direction) {
		case Up:
			snake_body->loc.y--;
			break;
		case Down:
			snake_body->loc.y++;
			break;
		case Left:
			snake_body->loc.x--;
			break;
		case Right:
			snake_body->loc.x++;
			break;
		default:
			break;
	}
	
	do {
		Snake *snake_tmp = NULL;
		while(move_snake->next != NULL) {
			snake_tmp = move_snake;
			move_snake->loc = move_snake->next->loc;
			move_snake = move_snake->next;
		}
		if(move_snake == snake_head) {
			free(move_snake);
			snake_head = NULL;
		} else {
			snake_tmp->next = NULL;
			free(move_snake);
		}
		snake_body->next = snake_head;
		snake_head = snake_body;
	}while(0);
	
	return Snake_Game_No_Error;
}

