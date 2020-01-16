/*************************************************************************//**
 *****************************************************************************
 * @file   console.h
 * @brief  
 * @author Forrest Y. Yu
 * @date   2005
 *****************************************************************************
 *****************************************************************************/

/**********************************************************
*	console.h       //added by mingxuan 2019-5-17
***********************************************************/

#ifndef	_ORANGES_CONSOLE_H_
#define	_ORANGES_CONSOLE_H_


/* CONSOLE */
typedef struct s_console
{
	unsigned int	crtc_start; /* set CRTC start addr reg */
	unsigned int	orig;	    /* start addr of the console */
	unsigned int	con_size;   /* how many words does the console have */
	unsigned int	cursor;
	int		is_full;
	unsigned int 	current_line;
}CONSOLE;


#define SCR_UP	1	/* scroll upward */
#define SCR_DN	-1	/* scroll downward */

#define SCR_SIZE		(80 * 25)
#define SCR_WIDTH		 80

#define DEFAULT_CHAR_COLOR	(MAKE_COLOR(BLACK, WHITE))
#define GRAY_CHAR		(MAKE_COLOR(BLACK, BLACK) | BRIGHT)
#define RED_CHAR		(MAKE_COLOR(BLUE, RED) | BRIGHT)


#endif /* _ORANGES_CONSOLE_H_ */