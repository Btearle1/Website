#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

/* Space Adventure Game - Brayden Tearle
   Text adventure that runs on the STM32 (Cortex-M4) over UART.

   The chapter 2 puzzles actually run the ARM instructions with inline asm
   instead of me just hardcoding 25 and 81, so if I ever change the
   instructions in the prompt I have to change them here too. */

/* easy mode: 20 + 5 = 25 */
int32_t easy_puzzle_asm(void) {
    int32_t result;
    __asm volatile (
        ".syntax unified \n\t"
        "MOV r0, #20    \n\t"
        "MOV r1, #5     \n\t"
        "ADD r0, r1, r0 \n\t"
        "MOV %[out], r0 \n\t"   // copy r0 back out into a C variable
        : [out] "=r" (result)
        :
        : "r0", "r1"   // tell the compiler I trashed r0 and r1 so it doesnt keep stuff there
    );
    return result;
}

/* hard mode: 20 << 2 = 80, then +1 = 81. LSL by 2 is the same as multiplying by 4 */
int32_t hard_puzzle_asm(void) {
    int32_t result;
    __asm volatile (
        ".syntax unified \n\t"
        "MOV r0, #20    \n\t"
        "LSL r0, r0, #2 \n\t"
        "ADD r0, r0, #1 \n\t"
        "MOV %[out], r0 \n\t"
        : [out] "=r" (result)
        :
        : "r0"
    );
    return result;
}

// ascii art (made these with an online ascii generator)
char* Blastoff_image =
"-----------------------------------------------------------==------------------- \n"
"------------------------------------------------------=#%#*#%=------------------ \n"
"---------------------------------------------------+%#++####%#------------------ \n"
"------------------------------------------------=%*+=+**####%%=----------------- \n"
"----------------------------------------------##*+==*****###%%#----------------- \n"
"-----------------------------------------=--%=**+-++*****###%%#----------------- \n"
"------------------------------------------#===--+###*****###%%#=---------------- \n"
"----------------------------------------++==--=====+#%%#**#%%%#*---------------- \n"
"-------------------------------------=-#==---==========*%%%@@@%#---------------- \n"
"-------------------------------------++==--===++++++=+++++++*#*#---------------- \n"
"------------------------------------#==----++**###***#++++++*#*#---------------- \n"
"-----------------------------------#==----+*#@#++++%@**#+++*##**---------------- \n"
"----------------------------------*-=----+*##======++%#*#++*#**+---------------- \n"
"---------------------------------*-=---==+*@*++++=+++*@+#*+*#**----------------- \n"
"--------------------------------*==----==+*@#*++++++*#%*%***#+%----------------- \n"
"-------------------------------#==----===+*#%*******#@*#%+*##*#----------------- \n"
"----------------------------=*#+==---=====***%%####%%*#%***#*#------------------ \n"
"--------------------==+#*=++#%%==---========##******#%*++*#*+#------------------ \n"
"------------------**++**+*%%%%===--============+***+++++**#+#=------------------ \n"
"-----------------#+#**#**%%%%#===-================+++++**#***------------------- \n"
"---------------=#*####***%%%@===--===============+++++**#*+#-------------------- \n"
"--------------=%*#####**%%%%%============##*===++++++**#**%=-------------------- \n"
"--------------#*#%%##**#%%%@#=======+==#+%%%+=++++++**#*+#=--------------------- \n"
"-------------*#%%%%#####%@%%+=========*+#%%#+++++++**#**##*--------------------- \n"
"------------=%#%%%#%##%==--+#%===+=++++#%%%#++++++**#**%%%*#--=----------------- \n"
"------------##%%#%###=-----=@##%#+++*+##%%%#+++++**#**%%%%%=*------------------- \n"
"-----------+#%%%%#%=------*=+*#%%#%#+##%%%**+++***#**@%%%#**+#------------------ \n"
"-----------%%%%%#*------=#-**#**##%=##%%%#*+****##+%%@%%#*#**+#----------------- \n"
"----------=#%%%%=-------+##**#*###**#%%%@%%%%%%#**%@@%%#*###**#=---------------- \n"
"----------#%%%*-----------=%*####%+#%%%%%%%@%%%##-*@@@#*#####**%---------------- \n"
"----------%%%=------------=*+*%*%##%%%###%%%%**----=@##*###%##*#---------------- \n"
"---------=%#------------=*++====**%%%%#%%%%%@**-----##%###%##*%=---------------- \n"
"---------+*------------+#+++==-=#%#=+#%%##%@@#+-----##%##%%%*%------------------ \n"
"----------------------=#+**+====--====+#=--=+=------##%%%%%##=------------------ \n"
"----------------------%##*++========+++#=-----------%#%%%%##=------------------- \n"
"---------------------=%*#*+++======+*+*#=----------=%%%%%##--------------------- \n"
"---------------------==##**++++++++*****-----------*#@%%##---------------------- \n"
"-----------------------###*++++++***#**------------%%%%#*----------------------- \n"
"-----------------------*##**+++**##+#*-------------%@%%=------------------------ \n"
"------------------------%##****###=#=-------------#%%#-------------------------- \n"
"------------------------##%###%#=-----------------@%+--------------------------- \n"
"------------------------*%#%%*-------------------#%----------------------------- \n"
"------------------------%#+----------------------------------------------------- \n"
"--------------------------------------------------------------------------------\n";

char *logo =
"      :*##*=    :*+.   -****.    -********.    +***-        \n"
"      +#+ +#+   :#*.  -#*==##   ##*+====+=.   *#*+*#=       \n"
"      +#+ .*#:  :#*.  *#=  +#* :#*-          =#*. -##.      \n"
"      +#+  -#*  :#*. +#*   .*#- :########+. .##=   +#*      \n"
"      +#+   +#- :#*:-*#:    =#*.        =#*.*#+    .*#=     \n"
"      +#+   :*#-+#*=+#+      +#*====---=*#++#*.     -#*:    \n"
"      :-:     =#*=.:==        ++++++++++-..+*=       =*=     \n";
char *victory =
"      _      _                   \n"
"     (_)    | |                  \n"
"__   ___  ___| |_ ___  _ __ _   _ \n"
"\\ \\ / / |/ __| __/ _ \\| '__| | | |\n"
" \\ V /| | (__| || (_) | |  | |_| |\n"
"  \\_/ |_|\\___|\\__\\___/|_|   \\__, |\n"
"                             __/ |\n"
"                            |___/ \n";
// globals so game() and set_pronouns() can both get to them
char name[64];
char gender[16];
char pronoun_subject[16];    // he / she / they
char pronoun_object[16];     // him / her / them
char pronoun_possessive[16]; // his / her / their
int current_location = 0;    // 0 = still on our ship, 1 = docked with the other ship
int puzzle1_solved = 0;      // ch 2 asteroid puzzle
int puzzle2_solved = 0;      // ch 3 docking puzzle
int puzzle3_solved = 0;      // ch 4 debris puzzle

// sets he/him/his etc so the crew dialogue matches what the player picked
void set_pronouns(void) {
    if (strcmp(gender, "male") == 0) {
        strcpy(pronoun_subject, "he");
        strcpy(pronoun_object, "him");
        strcpy(pronoun_possessive, "his");
    } else if (strcmp(gender, "female") == 0) {
        strcpy(pronoun_subject, "she");
        strcpy(pronoun_object, "her");
        strcpy(pronoun_possessive, "her");
    } else {
        // anything that isnt male/female just gets they/them
        strcpy(pronoun_subject, "they");
        strcpy(pronoun_object, "them");
        strcpy(pronoun_possessive, "their");
    }
}


int game(void); // declared up here because main() calls it before its defined

int main(void) {
    printf("%s", logo);
    char mminput[64];
    printf("Welcome to the Space Adventure Game!\n");
    printf("In this game, you will take on the role of a spaceship captain, making critical decisions that will affect the outcome of your mission.\n");
    printf("You will be faced with various challenges and obstacles, and your choices will determine the fate of your crew and the success of your mission.\n");

    while(1) {
        printf("\nAre you ready to begin your adventure? If so, please pick from the options below:\n");
        printf("1. Enter game.\n");
        printf("2. Help.\n");
        printf("3. Exit game.\n> ");
        scanf("%63s", mminput);   // %63s so it cant overflow the 64 char buffer

        if (strcmp(mminput, "1") == 0) {
            printf("\n*** IMPORTANT: When prompted for an answer or command, you must type EXACTLY what is requested -- nothing more, nothing less. Extra words, punctuation, or formatting will be treated as an incorrect answer. ***\n");
            printf("\nGreat! Let's get started.\n\n");
            game(); // the actual game
            break;
        } else if (strcmp(mminput, "2") == 0) {
            printf("\nHelp: In this game, you will be given decisions across your story that will impact the outcome of your playing experience. Choose wisely!\n");
        } else if (strcmp(mminput, "3") == 0) {
            printf("Exiting game. Goodbye!\n");
            return 0;
        } else {
            printf("Invalid input. Please try again.\n");
        }
    }
    return 0;
}



int game(void) {
    char input[64];
    int difficulty = -1; // -1 = not picked yet, 0 = easy, 1 = hard

    // reset everything in case this isnt the first playthrough
    current_location = 0;
    puzzle1_solved = 0;
    puzzle2_solved = 0;
    puzzle3_solved = 0;

    // character creation
    printf("=== CHARACTER CREATION ===\n");
    printf("Enter Captain's Surname: ");
    scanf("%63s", name);
    printf("Enter Captain's Gender (male, female, other): ");
    scanf("%15s", gender);

    // pick pronouns based on what they typed
    set_pronouns();

    printf("\n--- Chapter 1: The Briefing ---\n");
    printf("Welcome to the bridge, Captain %s.\n\n", name);
    printf("Your eyes slowly force themselves open and your right hand reaches for the alarm clock.\n"
           "You glance at the time and slowly get yourself ready for the day ahead.\n"
           "You walk out of your sleeping quarters and head to the bridge.\n"
           "You see your crew members already hard at work, and you take your place among them.\n\n");

    printf("\"Good morning, Captain %s. How was your rest?\" your Engineer asks.\n"
           "You respond with a nod and a smile, acknowledging their greeting.\n"
           "The ship's systems are running smoothly, and you begin your daily briefing with the crew.\n\n", name);

    printf("You briefly start the briefing, discussing the current status of the ship and any upcoming "
           "missions or tasks that need to be completed.\n"
           "Not long into the briefing, you receive a message from the ship's AI.\n"
           "\"Captain, an urgent message awaits your attention,\" the AI announces, informing you of an "
           "incoming transmission from a nearby friendly vessel.\n\n");

    printf("You quickly respond to the transmission, and a holographic image of the ship's captain "
           "appears before you.\n"
           "Their voice is urgent and they inform you about a leak in their ship's hull that is causing "
           "a dangerous situation.\n"
           "You quickly assess the situation and talk with your engineer before giving them instructions "
           "on how to stabilize the leak until your crew can arrive to assist them.\n\n");

    while (1) {
        printf("You sit down in your captain's chair and think. What do you do next?\n"
               "Type 'fly_now' to head to the ship immediately (Hard mode)\n"
               "Type 'talk_with_team' to confer with your crew first (Easy mode)\n> ");
        scanf("%63s", input);

        if (strcmp(input, "fly_now") == 0) {
            difficulty = 1; // hard
            break;
        } else if (strcmp(input, "talk_with_team") == 0) {
            difficulty = 0; // easy
            break;
        } else {
            printf("\nWhat was that? Please try again.\n\n");
        }
    }

    printf("\n");
    if (difficulty == 1) {
        // "they is" sounded wrong so pick the verb based on the pronoun
        printf("The Engineer mutters, \"I hope %s %s what %s %s doing...\"\n",
               pronoun_subject, strcmp(pronoun_subject, "they") == 0 ? "know" : "knows",
               pronoun_subject, strcmp(pronoun_subject, "they") == 0 ? "are" : "is");
        printf("You quickly sit in your captain's chair and prepare for the journey ahead.\n");
        printf("You give the order to set a course for the distressed ship, and your crew inputs the coordinates. Three... Two... One... Blastoff!\n");
        printf("%s", Blastoff_image);
    } else if (difficulty == 0) {
        printf("The AI notes, \"A wise choice, Captain. Giving the team %s full brief reduces operational risk.\"\n", pronoun_possessive);
        printf("You decide to confer with your crew first, discovering that the path to the distressed ship involves crossing a dangerous asteroid field.\n");
        printf("After a brief discussion, you develop a gameplan and give the order to set a course for the distressed ship.\n Three...\n Two...\n One...\n Blastoff!\n");
        printf("%s", Blastoff_image);
    }

    // ===== Chapter 2: asteroid field =====
    if (difficulty == 1) {
        printf("\n--- Chapter 2: The Asteroid Field ---\n");
        printf("Your ship unexpectedly enters the asteroid field...\n");
        printf("What is the decimal value of R0 after the following commands?\n"
               "MOV R0, #20\nLSL R0, R0, #2\nADD R0, R0, #1\n> ");
        strcpy(input, "");
        scanf("%63s", input);
        if (atoi(input) == hard_puzzle_asm()) {   // compare against what the asm actually computed
            printf("\nCorrect! The ship is repaired and you continue on your journey.\n");
            puzzle1_solved = 1;
        } else {
            printf("\nIncorrect! The correct answer was %ld. Game Over.\n", (long)hard_puzzle_asm());
            printf("Please press Enter to return to the main menu.\n");
            while (getchar() != '\n');   // clear out the leftover newline from scanf
            getchar();
            return 0;
        }
    }

    if (difficulty == 0) {
        printf("\n--- Chapter 2: The Asteroid Field ---\n");
        printf("Your ship enters the asteroid field...\n");
        printf("What is the decimal value of R0 after the following commands?\n"
               "MOV R0, #20\nMOV R1, #5\nADD R0, R1, R0\n> ");
        strcpy(input, "");
        scanf("%63s", input);
        if (atoi(input) == easy_puzzle_asm()) {
            printf("\nCorrect! You navigate through the asteroid field.\n");
            puzzle1_solved = 1;
        } else {
            printf("\nIncorrect! The correct answer was %ld. Game Over.\n", (long)easy_puzzle_asm());
            printf("Please press Enter to return to the main menu.\n");
            while (getchar() != '\n');
            getchar();
            return 0;
        }
    }

    // ===== Chapter 3: the distressed ship =====
    // both difficulties go through this one
    printf("\n--- Chapter 3: The Distressed Ship ---\n");
    printf("You arrive at the distressed ship and find it in critical condition.\n"
           "You rally your crew and prepare to dock.\n");
    printf("You must answer the following question to successfully dock:\n\n");

    if (difficulty == 1) {
        // 8 bits unsigned -> 2^8 - 1 = 255
        printf("The architecture is 8-bit. What is the max unsigned integer value?\n> ");
        strcpy(input, "");
        scanf("%63s", input);
        if (strcmp(input, "255") == 0) {
            printf("\nCorrect! You dock and save the crew. Congratulations!\n");
            puzzle2_solved = 1;
            current_location = 1; // docked
        } else {
            printf("\nIncorrect! The correct answer was 255. Game Over.\n");
            printf("Please press Enter to return to the main menu.\n");
            while (getchar() != '\n');
            getchar();
            return 0;
        }
    }

    if (difficulty == 0) {
        // 4 bits unsigned -> 2^4 - 1 = 15
        printf("The architecture is 4-bit. What is the max unsigned integer value?\n> ");
        strcpy(input, "");
        scanf("%63s", input);
        if (strcmp(input, "15") == 0) {
            printf("\nCorrect! You dock and save the crew. Congratulations!\n");
            puzzle2_solved = 1;
            current_location = 1; // docked
        } else {
            printf("\nIncorrect! The correct answer was 15. Game Over.\n");
            printf("Please press Enter to return to the main menu.\n");
            while (getchar() != '\n');
            getchar();
            return 0;
        }
    }

    // ===== Chapter 4: the rescue =====
    // also shared by both modes
    printf("\n--- Chapter 4: The Rescue ---\n");
    printf("\nYou have successfully docked the distressed ship and saved the crew. It is now time to board the ship and save the crew and their samples!\n");
    printf("You gather your crew and prepare to board the ship. As you enter, you find the crew members in a state of panic, but they are relieved to see you.\n");
    printf("You divide your crew into three teams: one to assist the crew, one to secure the samples, and one to evacuate the ship.\n");
    printf("After a few tense moments, you successfully secure the samples and evacuate the crew. You return to your ship and set a course for home.\n");
    printf("You sit back down in your captain's chair and once everyone is settled in, you release your ship from theirs and from the view of your window, you see the abandoned ship explode!\n");
    printf("Debris begin flying at your ship and it's now up to you to get out of there quick!\n");
    printf("You've practiced your emergency launches thousands of times and knew what to do! You grab your thrusters knob and slam it to full power!\n");
    printf("Your thrusters roar to life and you now must fly through the debris by answering the following question!\n");

    if (difficulty == 0) {
        printf("\n");
        // ohms law: series so R = 5 + 5 = 10, I = V/R = 10/10 = 2 A
        printf("A 10 volt voltage source is connected to two 5 ohm resistors in SERIES. What is the current flowing through the circuit?\n> ");
        strcpy(input, "");
        scanf("%63s", input);
        if (strcmp(input, "2") == 0) {
            printf("\nCorrect! You thread the needle through the debris field and make it out safely!\n");
            puzzle3_solved = 1;
              printf("%s", victory);
        } else {
            printf("\nIncorrect! The correct answer was 2. Game Over.\n");
            printf("Please press Enter to return to the main menu.\n");
            while (getchar() != '\n');
            getchar();
            return 0;
        }
    }

    if (difficulty == 1) {
        printf("\n");
        // I = 25/10 = 2.5 A. using strcmp here since atoi would chop off the .5
        printf("A 25 volt voltage source is connected to two 5 ohm resistors in SERIES. What is the current flowing through the circuit?\n> ");
        strcpy(input, "");
        scanf("%63s", input);
        if (strcmp(input, "2.5") == 0) {
            printf("\nCorrect! You thread the needle through the debris field and make it out safely!\n");
              printf("%s", victory);
            puzzle3_solved = 1;
        } else {
            printf("\nIncorrect! The correct answer was 2.5. Game Over.\n");
            printf("Please press Enter to return to the main menu.\n");
            while (getchar() != '\n');
            getchar();
            return 0;
        }
    }
 
    printf("\nYou make it home safely. Congratulations, Captain %s -- mission complete!\n", name);
 
    return 0;
}