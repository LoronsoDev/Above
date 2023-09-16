#pragma once

//ADAPTED FROM GLFW3.h

/* The unknown key */
#define AB_KEY_UNKNOWN            -1

/* Printable keys */
#define AB_KEY_SPACE              32
#define AB_KEY_APOSTROPHE         39  /* ' */
#define AB_KEY_COMMA              44  /* , */
#define AB_KEY_MINUS              45  /* - */
#define AB_KEY_PERIOD             46  /* . */
#define AB_KEY_SLASH              47  /* / */
#define AB_KEY_0                  48
#define AB_KEY_1                  49
#define AB_KEY_2                  50
#define AB_KEY_3                  51
#define AB_KEY_4                  52
#define AB_KEY_5                  53
#define AB_KEY_6                  54
#define AB_KEY_7                  55
#define AB_KEY_8                  56
#define AB_KEY_9                  57
#define AB_KEY_SEMICOLON          59  /* ; */
#define AB_KEY_EQUAL              61  /* = */
#define AB_KEY_A                  65
#define AB_KEY_B                  66
#define AB_KEY_C                  67
#define AB_KEY_D                  68
#define AB_KEY_E                  69
#define AB_KEY_F                  70
#define AB_KEY_G                  71
#define AB_KEY_H                  72
#define AB_KEY_I                  73
#define AB_KEY_J                  74
#define AB_KEY_K                  75
#define AB_KEY_L                  76
#define AB_KEY_M                  77
#define AB_KEY_N                  78
#define AB_KEY_O                  79
#define AB_KEY_P                  80
#define AB_KEY_Q                  81
#define AB_KEY_R                  82
#define AB_KEY_S                  83
#define AB_KEY_T                  84
#define AB_KEY_U                  85
#define AB_KEY_V                  86
#define AB_KEY_W                  87
#define AB_KEY_X                  88
#define AB_KEY_Y                  89
#define AB_KEY_Z                  90
#define AB_KEY_LEFT_BRACKET       91  /* [ */
#define AB_KEY_BACKSLASH          92  /* \ */
#define AB_KEY_RIGHT_BRACKET      93  /* ] */
#define AB_KEY_GRAVE_ACCENT       96  /* ` */
#define AB_KEY_WORLD_1            161 /* non-US #1 */
#define AB_KEY_WORLD_2            162 /* non-US #2 */

/* Function keys */
#define AB_KEY_ESCAPE             256
#define AB_KEY_ENTER              257
#define AB_KEY_TAB                258
#define AB_KEY_BACKSPACE          259
#define AB_KEY_INSERT             260
#define AB_KEY_DELETE             261
#define AB_KEY_RIGHT              262
#define AB_KEY_LEFT               263
#define AB_KEY_DOWN               264
#define AB_KEY_UP                 265
#define AB_KEY_PAGE_UP            266
#define AB_KEY_PAGE_DOWN          267
#define AB_KEY_HOME               268
#define AB_KEY_END                269
#define AB_KEY_CAPS_LOCK          280
#define AB_KEY_SCROLL_LOCK        281
#define AB_KEY_NUM_LOCK           282
#define AB_KEY_PRINT_SCREEN       283
#define AB_KEY_PAUSE              284
#define AB_KEY_F1                 290
#define AB_KEY_F2                 291
#define AB_KEY_F3                 292
#define AB_KEY_F4                 293
#define AB_KEY_F5                 294
#define AB_KEY_F6                 295
#define AB_KEY_F7                 296
#define AB_KEY_F8                 297
#define AB_KEY_F9                 298
#define AB_KEY_F10                299
#define AB_KEY_F11                300
#define AB_KEY_F12                301
#define AB_KEY_F13                302
#define AB_KEY_F14                303
#define AB_KEY_F15                304
#define AB_KEY_F16                305
#define AB_KEY_F17                306
#define AB_KEY_F18                307
#define AB_KEY_F19                308
#define AB_KEY_F20                309
#define AB_KEY_F21                310
#define AB_KEY_F22                311
#define AB_KEY_F23                312
#define AB_KEY_F24                313
#define AB_KEY_F25                314
#define AB_KEY_KP_0               320
#define AB_KEY_KP_1               321
#define AB_KEY_KP_2               322
#define AB_KEY_KP_3               323
#define AB_KEY_KP_4               324
#define AB_KEY_KP_5               325
#define AB_KEY_KP_6               326
#define AB_KEY_KP_7               327
#define AB_KEY_KP_8               328
#define AB_KEY_KP_9               329
#define AB_KEY_KP_DECIMAL         330
#define AB_KEY_KP_DIVIDE          331
#define AB_KEY_KP_MULTIPLY        332
#define AB_KEY_KP_SUBTRACT        333
#define AB_KEY_KP_ADD             334
#define AB_KEY_KP_ENTER           335
#define AB_KEY_KP_EQUAL           336
#define AB_KEY_LEFT_SHIFT         340
#define AB_KEY_LEFT_CONTROL       341
#define AB_KEY_LEFT_ALT           342
#define AB_KEY_LEFT_SUPER         343
#define AB_KEY_RIGHT_SHIFT        344
#define AB_KEY_RIGHT_CONTROL      345
#define AB_KEY_RIGHT_ALT          346
#define AB_KEY_RIGHT_SUPER        347
#define AB_KEY_MENU               348

#define AB_KEY_LAST               AB_KEY_MENU

/*! @} */

/*! @defgroup mods Modifier key flags
 *  @brief Modifier key flags.
 *
 *  See [key input](@ref input_key) for how these are used.
 *
 *  @ingroup input
 *  @{ */

 /*! @brief If this bit is set one or more Shift keys were held down.
  *
  *  If this bit is set one or more Shift keys were held down.
  */
#define AB_MOD_SHIFT           0x0001
  /*! @brief If this bit is set one or more Control keys were held down.
   *
   *  If this bit is set one or more Control keys were held down.
   */
#define AB_MOD_CONTROL         0x0002
   /*! @brief If this bit is set one or more Alt keys were held down.
	*
	*  If this bit is set one or more Alt keys were held down.
	*/
#define AB_MOD_ALT             0x0004
	/*! @brief If this bit is set one or more Super keys were held down.
	 *
	 *  If this bit is set one or more Super keys were held down.
	 */
#define AB_MOD_SUPER           0x0008
	 /*! @brief If this bit is set the Caps Lock key is enabled.
	  *
	  *  If this bit is set the Caps Lock key is enabled and the @ref
	  *  AB_LOCK_KEY_MODS input mode is set.
	  */
#define AB_MOD_CAPS_LOCK       0x0010
	  /*! @brief If this bit is set the Num Lock key is enabled.
	   *
	   *  If this bit is set the Num Lock key is enabled and the @ref
	   *  AB_LOCK_KEY_MODS input mode is set.
	   */
#define AB_MOD_NUM_LOCK        0x0020


#define AB_JOYSTICK_1             0
#define AB_JOYSTICK_2             1
#define AB_JOYSTICK_3             2
#define AB_JOYSTICK_4             3
#define AB_JOYSTICK_5             4
#define AB_JOYSTICK_6             5
#define AB_JOYSTICK_7             6
#define AB_JOYSTICK_8             7
#define AB_JOYSTICK_9             8
#define AB_JOYSTICK_10            9
#define AB_JOYSTICK_11            10
#define AB_JOYSTICK_12            11
#define AB_JOYSTICK_13            12
#define AB_JOYSTICK_14            13
#define AB_JOYSTICK_15            14
#define AB_JOYSTICK_16            15
#define AB_JOYSTICK_LAST          AB_JOYSTICK_16

		 /*! @} */

		 /*! @defgroup gamepad_buttons Gamepad buttons
		  *  @brief Gamepad buttons.
		  *
		  *  See @ref gamepad for how these are used.
		  *
		  *  @ingroup input
		  *  @{ */
#define AB_GAMEPAD_BUTTON_A               0
#define AB_GAMEPAD_BUTTON_B               1
#define AB_GAMEPAD_BUTTON_X               2
#define AB_GAMEPAD_BUTTON_Y               3
#define AB_GAMEPAD_BUTTON_LEFT_BUMPER     4
#define AB_GAMEPAD_BUTTON_RIGHT_BUMPER    5
#define AB_GAMEPAD_BUTTON_BACK            6
#define AB_GAMEPAD_BUTTON_START           7
#define AB_GAMEPAD_BUTTON_GUIDE           8
#define AB_GAMEPAD_BUTTON_LEFT_THUMB      9
#define AB_GAMEPAD_BUTTON_RIGHT_THUMB     10
#define AB_GAMEPAD_BUTTON_DPAD_UP         11
#define AB_GAMEPAD_BUTTON_DPAD_RIGHT      12
#define AB_GAMEPAD_BUTTON_DPAD_DOWN       13
#define AB_GAMEPAD_BUTTON_DPAD_LEFT       14
#define AB_GAMEPAD_BUTTON_LAST            AB_GAMEPAD_BUTTON_DPAD_LEFT

#define AB_GAMEPAD_BUTTON_CROSS       AB_GAMEPAD_BUTTON_A
#define AB_GAMEPAD_BUTTON_CIRCLE      AB_GAMEPAD_BUTTON_B
#define AB_GAMEPAD_BUTTON_SQUARE      AB_GAMEPAD_BUTTON_X
#define AB_GAMEPAD_BUTTON_TRIANGLE    AB_GAMEPAD_BUTTON_Y
		  /*! @} */

		  /*! @defgroup gamepad_axes Gamepad axes
		   *  @brief Gamepad axes.
		   *
		   *  See @ref gamepad for how these are used.
		   *
		   *  @ingroup input
		   *  @{ */
#define AB_GAMEPAD_AXIS_LEFT_X        0
#define AB_GAMEPAD_AXIS_LEFT_Y        1
#define AB_GAMEPAD_AXIS_RIGHT_X       2
#define AB_GAMEPAD_AXIS_RIGHT_Y       3
#define AB_GAMEPAD_AXIS_LEFT_TRIGGER  4
#define AB_GAMEPAD_AXIS_RIGHT_TRIGGER 5
#define AB_GAMEPAD_AXIS_LAST          AB_GAMEPAD_AXIS_RIGHT_TRIGGER