/******************************************************************************

                  Copyright (c) 2017 Siddharth Chandrasekaran

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.

    Author : Siddharth Chandrasekaran
    Email  : siddharth@embedjournal.com
    Date   : 07-May-2016

******************************************************************************/

#include "fonts.h"

/* 
**  Font data for Arial 24pt
*/

/* Character bitmaps for Arial 24pt */
const uint8_t arial_24ptBitmaps[] = 
{
	/* @0 '0' (16 pixels wide) */
	0x07, 0xE0, //      ######     
	0x1F, 0xF8, //    ##########   
	0x3F, 0xFC, //   ############  
	0x7C, 0x3C, //  #####    ####  
	0x70, 0x0E, //  ###        ### 
	0x70, 0x0E, //  ###        ### 
	0xE0, 0x07, // ###          ###
	0xE0, 0x07, // ###          ###
	0xE0, 0x07, // ###          ###
	0xE0, 0x07, // ###          ###
	0xE0, 0x07, // ###          ###
	0xE0, 0x07, // ###          ###
	0xE0, 0x07, // ###          ###
	0xE0, 0x07, // ###          ###
	0xE0, 0x07, // ###          ###
	0xE0, 0x07, // ###          ###
	0xE0, 0x07, // ###          ###
	0x70, 0x0E, //  ###        ### 
	0x70, 0x0E, //  ###        ### 
	0x7C, 0x3E, //  #####    ##### 
	0x3F, 0xFC, //   ############  
	0x1F, 0xF8, //    ##########   
	0x07, 0xE0, //      ######     

	/* @46 '1' (9 pixels wide) */
	0x01, 0x80, //        ##
	0x03, 0x80, //       ###
	0x07, 0x80, //      ####
	0x0F, 0x80, //     #####
	0x3F, 0x80, //   #######
	0x7B, 0x80, //  #### ###
	0xF3, 0x80, // ####  ###
	0xC3, 0x80, // ##    ###
	0x03, 0x80, //       ###
	0x03, 0x80, //       ###
	0x03, 0x80, //       ###
	0x03, 0x80, //       ###
	0x03, 0x80, //       ###
	0x03, 0x80, //       ###
	0x03, 0x80, //       ###
	0x03, 0x80, //       ###
	0x03, 0x80, //       ###
	0x03, 0x80, //       ###
	0x03, 0x80, //       ###
	0x03, 0x80, //       ###
	0x03, 0x80, //       ###
	0x03, 0x80, //       ###
	0x03, 0x80, //       ###

	/* @92 '2' (15 pixels wide) */
	0x0F, 0xE0, //     #######    
	0x3F, 0xF8, //   ###########  
	0x7F, 0xFC, //  ############# 
	0x78, 0x3C, //  ####     #### 
	0xE0, 0x1E, // ###        ####
	0xE0, 0x0E, // ###         ###
	0x00, 0x0E, //             ###
	0x00, 0x0E, //             ###
	0x00, 0x0E, //             ###
	0x00, 0x1C, //            ### 
	0x00, 0x1C, //            ### 
	0x00, 0x38, //           ###  
	0x00, 0x70, //          ###   
	0x00, 0xE0, //         ###    
	0x01, 0xC0, //        ###     
	0x07, 0x80, //      ####      
	0x0F, 0x00, //     ####       
	0x1C, 0x00, //    ###         
	0x38, 0x00, //   ###          
	0x70, 0x00, //  ###           
	0x7F, 0xFE, //  ##############
	0xFF, 0xFE, // ###############
	0xFF, 0xFE, // ###############

	/* @138 '3' (16 pixels wide) */
	0x07, 0xE0, //      ######     
	0x1F, 0xF8, //    ##########   
	0x3F, 0xFC, //   ############  
	0x78, 0x3C, //  ####     ####  
	0xF0, 0x0E, // ####        ### 
	0xE0, 0x0E, // ###         ### 
	0x00, 0x0E, //             ### 
	0x00, 0x1E, //            #### 
	0x00, 0x3C, //           ####  
	0x03, 0xF8, //       #######   
	0x03, 0xF8, //       #######   
	0x03, 0xFC, //       ########  
	0x00, 0x1E, //            #### 
	0x00, 0x0F, //             ####
	0x00, 0x07, //              ###
	0x00, 0x07, //              ###
	0x00, 0x07, //              ###
	0xE0, 0x07, // ###          ###
	0xF0, 0x0E, // ####        ### 
	0x78, 0x1E, //  ####      #### 
	0x3F, 0xFC, //   ############  
	0x1F, 0xF8, //    ##########   
	0x07, 0xE0, //      ######     

	/* @184 '4' (16 pixels wide) */
	0x00, 0x18, //            ##   
	0x00, 0x38, //           ###   
	0x00, 0x78, //          ####   
	0x00, 0xF8, //         #####   
	0x00, 0xF8, //         #####   
	0x01, 0xF8, //        ######   
	0x03, 0xB8, //       ### ###   
	0x07, 0xB8, //      #### ###   
	0x07, 0x38, //      ###  ###   
	0x0E, 0x38, //     ###   ###   
	0x1C, 0x38, //    ###    ###   
	0x1C, 0x38, //    ###    ###   
	0x38, 0x38, //   ###     ###   
	0x70, 0x38, //  ###      ###   
	0xE0, 0x38, // ###       ###   
	0xFF, 0xFF, // ################
	0xFF, 0xFF, // ################
	0xFF, 0xFF, // ################
	0x00, 0x38, //           ###   
	0x00, 0x38, //           ###   
	0x00, 0x38, //           ###   
	0x00, 0x38, //           ###   
	0x00, 0x38, //           ###   

	/* @230 '5' (16 pixels wide) */
	0x3F, 0xFE, //   ############# 
	0x3F, 0xFE, //   ############# 
	0x3F, 0xFE, //   ############# 
	0x70, 0x00, //  ###            
	0x70, 0x00, //  ###            
	0x70, 0x00, //  ###            
	0x70, 0x00, //  ###            
	0x73, 0xE0, //  ###  #####     
	0x6F, 0xF8, //  ## #########   
	0xFF, 0xFC, // ##############  
	0xF8, 0x1E, // #####      #### 
	0xE0, 0x0E, // ###         ### 
	0x00, 0x07, //              ###
	0x00, 0x07, //              ###
	0x00, 0x07, //              ###
	0x00, 0x07, //              ###
	0x00, 0x07, //              ###
	0xE0, 0x07, // ###          ###
	0xF0, 0x0E, // ####        ### 
	0x78, 0x1E, //  ####      #### 
	0x7F, 0xFC, //  #############  
	0x1F, 0xF8, //    ##########   
	0x0F, 0xE0, //     #######     

	/* @276 '6' (16 pixels wide) */
	0x03, 0xF0, //       ######    
	0x0F, 0xFC, //     ##########  
	0x1F, 0xFE, //    ############ 
	0x3C, 0x1E, //   ####     #### 
	0x78, 0x0F, //  ####       ####
	0x70, 0x07, //  ###         ###
	0x70, 0x00, //  ###            
	0xE0, 0x00, // ###             
	0xE3, 0xF0, // ###   ######    
	0xEF, 0xF8, // ### #########   
	0xFF, 0xFC, // ##############  
	0xFC, 0x1E, // ######     #### 
	0xF0, 0x0F, // ####        ####
	0xE0, 0x07, // ###          ###
	0xE0, 0x07, // ###          ###
	0xE0, 0x07, // ###          ###
	0xE0, 0x07, // ###          ###
	0x70, 0x07, //  ###         ###
	0x70, 0x0E, //  ###        ### 
	0x3C, 0x1E, //   ####     #### 
	0x3F, 0xFC, //   ############  
	0x0F, 0xF8, //     #########   
	0x07, 0xE0, //      ######     

	/* @322 '7' (15 pixels wide) */
	0xFF, 0xFE, // ###############
	0xFF, 0xFE, // ###############
	0xFF, 0xFE, // ###############
	0x00, 0x0C, //             ## 
	0x00, 0x18, //            ##  
	0x00, 0x38, //           ###  
	0x00, 0x70, //          ###   
	0x00, 0xE0, //         ###    
	0x00, 0xE0, //         ###    
	0x01, 0xC0, //        ###     
	0x01, 0xC0, //        ###     
	0x03, 0x80, //       ###      
	0x03, 0x80, //       ###      
	0x07, 0x00, //      ###       
	0x07, 0x00, //      ###       
	0x0E, 0x00, //     ###        
	0x0E, 0x00, //     ###        
	0x0E, 0x00, //     ###        
	0x0E, 0x00, //     ###        
	0x1C, 0x00, //    ###         
	0x1C, 0x00, //    ###         
	0x1C, 0x00, //    ###         
	0x1C, 0x00, //    ###         

	/* @368 '8' (16 pixels wide) */
	0x07, 0xE0, //      ######     
	0x1F, 0xF8, //    ##########   
	0x3F, 0xFC, //   ############  
	0x78, 0x1C, //  ####      ###  
	0x70, 0x0E, //  ###        ### 
	0x70, 0x0E, //  ###        ### 
	0x70, 0x0E, //  ###        ### 
	0x70, 0x0E, //  ###        ### 
	0x38, 0x1C, //   ###      ###  
	0x1F, 0xF8, //    ##########   
	0x0F, 0xF0, //     ########    
	0x3F, 0xFC, //   ############  
	0x78, 0x1E, //  ####      #### 
	0x70, 0x0E, //  ###        ### 
	0xE0, 0x07, // ###          ###
	0xE0, 0x07, // ###          ###
	0xE0, 0x07, // ###          ###
	0xE0, 0x07, // ###          ###
	0xF0, 0x0F, // ####        ####
	0x78, 0x1E, //  ####      #### 
	0x3F, 0xFC, //   ############  
	0x1F, 0xF8, //    ##########   
	0x07, 0xE0, //      ######     

	/* @414 '9' (16 pixels wide) */
	0x07, 0xE0, //      ######     
	0x1F, 0xF0, //    #########    
	0x3F, 0xFC, //   ############  
	0x78, 0x1C, //  ####      ###  
	0x70, 0x0E, //  ###        ### 
	0xE0, 0x06, // ###          ## 
	0xE0, 0x07, // ###          ###
	0xE0, 0x07, // ###          ###
	0xE0, 0x07, // ###          ###
	0xE0, 0x07, // ###          ###
	0xF0, 0x0F, // ####        ####
	0x78, 0x1F, //  ####      #####
	0x3F, 0xFF, //   ##############
	0x1F, 0xF7, //    ######### ###
	0x0F, 0xC7, //     ######   ###
	0x00, 0x07, //              ###
	0x00, 0x0E, //             ### 
	0xE0, 0x0E, // ###         ### 
	0xF0, 0x1E, // ####       #### 
	0x78, 0x3C, //  ####     ####  
	0x7F, 0xF8, //  ############   
	0x3F, 0xF0, //   ##########    
	0x0F, 0xC0, //     ######      
};

const font_descriptor_t arial_24ptDescriptors[] = 
{
	{16, 0}, 		/* 0 */ 
	{9, 46}, 		/* 1 */ 
	{15, 92}, 		/* 2 */ 
	{16, 138}, 		/* 3 */ 
	{16, 184}, 		/* 4 */ 
	{16, 230}, 		/* 5 */ 
	{16, 276}, 		/* 6 */ 
	{15, 322}, 		/* 7 */ 
	{16, 368}, 		/* 8 */ 
	{16, 414}, 		/* 9 */ 
};

const font_t arial_24ptFontInfo =
{
	2,   /*  Space between chars */
	23,  /*  Character height */
	'0', /*  Start character */
	'9', /*  End character */
	arial_24ptDescriptors, /*  Character descriptor array */
	OBJ_COL_MAJOR,         /*  Type of bit packing */
	arial_24ptBitmaps,     /*  Character bitmap array */
};

