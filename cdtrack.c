/**************************************************************************** 
 *
 *   GLORY:
 *
 *   CDTRACK.C  0.01    Joseph Chiu, California Institute of Technology
 *                      Mail Box 380, Caltech, Pasadena, California 91126
 *   Written 5/11/93    josephc@legend.caltech.edu, josephc@cco.caltech.edu
 *                      (818) 449 5457          Compu$pend: 72113,2105
 *   
 *   CDTRACK, Copyright (C) 1993, Joseph Chiu, All Rights Reserved
 *
 *   Send me E-mail!  I'd like to hear from ya!
 *   (Bug fixes, suggestions, and usage anecdotes are very welcome!)
 *
 ****************************************************************************
 *
 *   INFO:
 *
 *   CDTRACK : Command line CD-player
 *
 *   Format:  CDTRACK <start> <stop>
 *
 *      where <start> and <stop> are track numbers.
 *
 *      if parameters are not given, CDTRACK will default to play track 1 only.
 *      if only <start> is given, CDTRACK will play that track only.
 *
 ****************************************************************************
 *
 *   LEGALESE:
 *
 *   This program is Freeware.  There is no fee for the use of this software;
 *   the author only asks that you send him e-mail if you find this software
 *   useful, or have ideas for improvements.  The author of this software makes
 *   no expressed or implied warranty of any kind with regards to this software
 *   and in no event will be liable for incidental or consequential damages
 *   arising from the use of this product, even if the author has been advised
 *   of the possibility of said damages. (Ah, the worries in sue-happy U.S.A.!)
 *
 ****************************************************************************
 *
 *  HISTORY:
 *   
 *  11-May-93  First release of CDTRACK.  Version number 0.01 because there's
 *             a lot more to go before it can even be called a product.
 *             Compiled under CSet++/2 March Beta
 *
 ****************************************************************************
 *
 * NOTES:
 *
 * If you get an error 'mciSendString - unresolved external', you need to make
 * sure that you have included the MMPM2 library in the library path, and/or
 * that it is specifically "given" to the linker
 *
 * This program is for command-line use only.  If you want a 'nice' PM-based
 * CD-player, get CDEXPL (CD-Explorer).  It is a much better product for end-
 * users than this program will ever be.
 *
 * To-Do:  Allow communicating via named pipes or queues to allow the parent
 * process to control CD-Track's behaviour (e.g., force CD-Track to stop early),
 * and put in 'Verbose' mode [yanked out, pending improvements].
 * 
 * "OS/2 is my programming environment of choice.  It allows me to break free
 *  from the constraints of the DOS+Windows environment." --Joseph Chiu.
 */

#include <os2.h>
#include <stdio.h>
#include <os2me.h>

#define STRING_SIZE 128
#define MCI_STRING_LENGTH 128

main(int argc, char *argv[], char *envp[])
{
   CHAR acTempString [STRING_SIZE],       /* Used to store command strings  */
        acMCIString [MCI_STRING_LENGTH];  /* Used for notification messages */

   LONG lSendStringRC = 0;                /* return code from mciSendString */

   int  trackstart,                       /* Starting and ending track #'s  */
        trackend;   


   if (argc > 1) {                        /* Get start/stop track numbers */
      trackstart = atoi (argv[1]);
      if (trackstart < 1)
         trackstart = 1;
   } else {
      trackstart = 1;
   } /* endif */

   if (argc <= 2) {
      trackend = trackstart;
   } else {
      trackend = atoi(argv[2]);
      if (trackend < trackstart)
        trackend = trackstart;
   } /* endif */



/***  Open the CDROM Drive for use with this program */

   strcpy (acTempString, "open cdaudio wait");
   lSendStringRC =
        mciSendString (
                (LPSTR) &acTempString[0],   /* The command to the MM-API */
                (LPSTR) NULL,               /* We don't expect an answer */
                (WORD) 0,                   /* 0-length return string */
                0,                          /* Do not notify any window */
                0);                         /* User Parameter */

   if (lSendStringRC != MCIERR_SUCCESS)
      return -1;


/*** Set the CDROM Drive to use the Track:Minute:Second:Frame format */

   strcpy (acTempString, "set cdaudio time format tmsf wait");

   lSendStringRC =
        mciSendString (
                (LPSTR) &acTempString[0],   
                (LPSTR) NULL,  
                (WORD) 0, 
                0,            
                0);       

   if (lSendStringRC != MCIERR_SUCCESS)
      return -1;


/*** Get the length of the last track's length in minute:second:frame format */

   sprintf (acTempString, "status cdaudio length track %d wait", trackend);
   lSendStringRC =
        mciSendString (
                (LPSTR) &acTempString[0],  /* The command */
                (LPSTR) &acMCIString[0],   /* Place to put return string */
                (WORD) MCI_STRING_LENGTH,  /* The size of the return space */
                0,                         /* No notifies */
                0);           

   if (lSendStringRC != MCIERR_SUCCESS)
      return -1;


/*** Play from the start-track to the end of the stop-track  */

   sprintf (acTempString, "play cdaudio from %02d:00:00:00 to %02d:%s wait",
                trackstart, trackend, acMCIString);

   lSendStringRC =
        mciSendString (
                (LPSTR) &acTempString[0],        /* The command */
                (LPSTR) NULL,  /* Place to put return string */
                (WORD) 0, /* How large is the return space */
                0,            /* Which window receives notifies */
                0);       /* User Parameter */

   if (lSendStringRC != MCIERR_SUCCESS)
      return -1;



/***  Stop the CDROM drive */   

   strcpy (acTempString, "stop cdaudio");
   lSendStringRC =
        mciSendString (
                (LPSTR) &acTempString[0],
                (LPSTR) NULL,  
                (WORD) 0, 
                0,
                0);

   if (lSendStringRC != MCIERR_SUCCESS)
      return -1;


/***  Close the CDROM drive */

   strcpy (acTempString, "close cdaudio wait");

   lSendStringRC =
        mciSendString (
                (LPSTR) &acTempString[0],        /* The command */
                (LPSTR) NULL,  /* Place to put return string */
                (WORD) 0, /* How large is the return space */
                0,            /* Which window receives notifies */
                0);       /* User Parameter */

   if (lSendStringRC != MCIERR_SUCCESS)
      return -1;

 
/*** Success! */

   return 0;
}


