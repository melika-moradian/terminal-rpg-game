#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

typedef struct {
    int hp;
    int money;
    int wins;
    int has_sword;
    int has_armor;
    int has_key;
} Player;

typedef struct {
    char name[20];
    int type;
    int dice_needed;
    int damage;
    int reward;
    int is_boss;
} Enemy;

Player saved_player;
int is_saved = 0;

void start_new_game(Player *p);
void save_game(Player *p);
void load_game(Player *p);
void show_menu();
void go_to_town(Player *p);
void choose_level(Player *p);
void play_level_1(Player *p);
void play_level_2(Player *p);
void play_level_3(Player *p);
void open_store(Player *p);
Enemy make_enemy_1(int dice);
Enemy make_enemy_2(int dice);
Enemy make_enemy_3(int dice);

int main() {
    srand(time(NULL));
    Player my_player;       
    int menu_choice;
    
    while(1) { 
        show_menu();
        if (scanf("%d", &menu_choice) != 1) { 
            while(getchar() != '\n');
            menu_choice = 0;
        }
        switch(menu_choice) {
            case 1:
                start_new_game(&my_player);
                go_to_town(&my_player);
                break;
            case 2:
                load_game(&my_player);
                if(is_saved == 1) {
                    go_to_town(&my_player);
                }
                break;
            case 3:
                printf("Goodbye!\n");
                return 0;
            default:
                printf("\nWrong number! Try again.\n");
        }
    }
    return 0; 
}

void show_menu() {
    printf("\n=== MY ADVENTURE GAME ===\n");
    printf("1. Start New Game\n");
    printf("2. Load Game\n");
    printf("3. Exit\n");
    printf("Choose: ");
}

void start_new_game(Player *p) {
    p->hp = 20;
    p->money = 0;
    p->wins = 0;
    p->has_sword = 0;
    p->has_armor = 0;
    p->has_key = 0;
    printf("\n---> New game started! Good luck.\n");
}

void save_game(Player *p) {
    saved_player = *p;
    is_saved = 1;
    printf("\n[System] Game saved!\n");
}

void load_game(Player *p) {
    if (is_saved == 1) {
        *p = saved_player;
        printf("\n[System] Game loaded!\n");
    } else {
        printf("\n[Error] No save found.\n");
    }
}

void go_to_town(Player *p) {
    int town_choice;
    int stay_in_town = 1;
    
    while(stay_in_town == 1) {
        printf("\n--- TOWN ---\n");
        printf("1. Go to a Level\n");
        printf("2. Sleep (HP goes to 20)\n");
        printf("3. Show Stats\n");
        printf("4. Save Game\n");
        printf("5. Back to Menu\n");
        printf("Action: ");
        
        if(scanf("%d", &town_choice) != 1) {
             while(getchar() != '\n');
             town_choice = 0;
        }

        switch(town_choice) {
            case 1: 
                choose_level(p);
                break;
            case 2: 
                p->hp = 20;
                printf("\n[Sleep] You feel better. HP is 20.\n");
                break;
            case 3:
                printf("\n[STATS] HP: %d | Money: %d\n", p->hp, p->money);
                printf("[ITEMS] Sword: %d | Armor: %d | Key: %d\n", p->has_sword, p->has_armor, p->has_key);
                break;
            case 4:
                save_game(p);
                break; 
            case 5: 
                stay_in_town = 0;
                break;
            default: 
                printf("Bad choice.\n");
        }
    }
}

void choose_level(Player *p) {
    printf("\n--- LEVELS ---\n");
    printf("1. Poison Swamp (Easy)\n");
    printf("2. Old Castle (Medium)\n");
    if (p->has_key == 1) {
        printf("3. Dark Cave (BOSS)\n");
    } else {
        printf("3. Dark Cave (Locked - Get the key in Level 2)\n");
    }
    
    printf("Select: ");
    int lvl;
    if(scanf("%d", &lvl) != 1) { while(getchar() != '\n'); lvl = 0; }
    
    if(lvl == 1) play_level_1(p);
    else if(lvl == 2) play_level_2(p);
    else if(lvl == 3) {
        if(p->has_key == 1) play_level_3(p);
        else printf("\n[Locked] You need the key!\n");
    } 
    else printf("Level not found!\n");
}

void open_store(Player *p) {
    printf("\n--- STORE ---\n");
    printf("1. Health Potion (4 money) -> Plus HP\n");
    printf("2. Sword (5 money)  -> Plus Attack\n");
    printf("3. Armor (10 money) -> Less Damage\n");
    printf("4. Exit\nBuy: ");
    
    int item;
    if(scanf("%d", &item) != 1) { while(getchar() != '\n'); item = 0; }
    
    if(item == 1 && p->money >= 4) { 
        p->money -= 4;
        int heal = (rand() % 6) + 1;
        p->hp += heal;
        if(p->hp > 20) p->hp = 20; 
        printf("Bought Potion. Healed %d. HP is %d\n", heal, p->hp); 
    }
    else if(item == 2 && p->money >= 5) { 
        if(p->has_sword == 0) {
            p->money -= 5; 
            p->has_sword = 1; 
            printf("Bought Sword!\n"); 
        } else printf("You already have a Sword!\n");
    }
    else if(item == 3 && p->money >= 10) { 
        if(p->has_armor == 0) {
            p->money -= 10; 
            p->has_armor = 1; 
            printf("Bought Armor!\n"); 
        } else printf("You already have Armor!\n");
    }
    else if(item == 4) {} 
    else printf("Not enough money or wrong number!\n");
}

Enemy make_enemy_1(int dice) {
    Enemy e;
    e.is_boss = 0;
    if (dice == 1) { strcpy(e.name, "Angry Dog"); e.type=1; e.dice_needed=2; e.damage=1; e.reward=1; }
    else if (dice == 2) { strcpy(e.name, "Goblin"); e.type=1; e.dice_needed=3; e.damage=2; e.reward=2; }
    else if (dice == 3) { strcpy(e.name, "Skeleton"); e.type=1; e.dice_needed=4; e.damage=2; e.reward=4; }
    else if (dice == 4) { strcpy(e.name, "Big Orc"); e.type=1; e.dice_needed=3; e.damage=4; e.reward=6; }
    else if (dice == 5) { strcpy(e.name, "Acid Trap"); e.type=2; e.dice_needed=0; e.damage=3; e.reward=0; } 
    else if (dice == 6) { strcpy(e.name, "Orc Boss"); e.type=1; e.dice_needed=3; e.damage=4; e.reward=10; e.is_boss=1; } 
    return e;
}

void play_level_1(Player *p) {
    printf("\n>>> POISON SWAMP <<<\n");
    int map[10];
    int boss_count = 0;
    
    for(int i = 0; i < 10; i++) { 
        map[i] = (rand() % 6) + 1; 
        if(map[i] == 6) boss_count++;
    }
    while(boss_count < 3) { 
        int pos = rand() % 10;
        if(map[pos] != 6) { map[pos] = 6; boss_count++; }
    }

    int room = 0, bosses_killed = 0, playing = 1;
    
    while(playing == 1 && p->hp > 0) {
        printf("\n[Level 1] Room: %d/10 | Bosses: %d/3 | HP: %d\n", room+1, bosses_killed, p->hp);
        printf("1. Walk | 2. Store | 3. Run Away (50 money)\nAction: ");
        
        int act; 
        if(scanf("%d", &act) != 1) { while(getchar() != '\n'); act = 0; }

        if(act == 1) {
            if(room >= 10) { playing = 0; break; }
            Enemy foe = make_enemy_1(map[room]);
            room++; 
            
            printf("--- Enemy: %s ---\n", foe.name);
            if(foe.type == 1) {
                printf("FIGHT! You need > %d\n", foe.dice_needed);
                while(1) {
                    int roll = (rand() % 6) + 1;
                    int my_attack = roll + p->has_sword;
                    printf(" > Dice: %d (Total Attack: %d)\n", roll, my_attack);
                    
                    if(my_attack > foe.dice_needed) {
                        printf(" > WIN! +%d money.\n", foe.reward);
                        p->money += foe.reward;
                        if(foe.is_boss == 1) { bosses_killed++; printf(" > BOSS DEAD!\n"); }
                        break;
                    }
                    int hurt = foe.damage - p->has_armor; 
                    if(hurt < 0) hurt = 0; 
                    printf(" > FAIL! You lost %d HP.\n", hurt);
                    p->hp -= hurt;
                    if(p->hp <= 0) { playing = 0; break; }
                }
            } 
            else if(foe.type == 2) {
                int trap_hurt = foe.damage - p->has_armor; 
                if(trap_hurt < 0) trap_hurt = 0; 
                printf("TRAP! You lost %d HP.\n", trap_hurt);
                p->hp -= trap_hurt;
            }
            if(bosses_killed >= 3 && p->hp > 0) {
                printf("\n*** LEVEL 1 FINISHED! ***\n");
                p->wins++;
                playing = 0;
            }
        } 
        else if(act == 2) open_store(p);
        else if(act == 3 && p->money >= 50) { p->money -= 50; playing = 0; }
        else printf("You can't do that!\n");
    }
}

Enemy make_enemy_2(int dice) {
    Enemy e; e.is_boss = 0;
    if (dice == 1) { strcpy(e.name, "Fire Trap"); e.type=2; e.dice_needed=0; e.damage=3; e.reward=0; }
    else if (dice == 2) { strcpy(e.name, "Bat"); e.type=1; e.dice_needed=2; e.damage=2; e.reward=1; }
    else if (dice == 3) { strcpy(e.name, "Zombie"); e.type=1; e.dice_needed=3; e.damage=2; e.reward=2; }
    else if (dice == 4) { strcpy(e.name, "Ghost"); e.type=1; e.dice_needed=5; e.damage=2; e.reward=4; }
    else if (dice == 5) { strcpy(e.name, "Vampire Boss"); e.type=1; e.dice_needed=4; e.damage=4; e.reward=7; e.is_boss=1; } 
    else if (dice == 6) { strcpy(e.name, "Demon"); e.type=1; e.dice_needed=4; e.damage=6; e.reward=10; }
    return e;
}

void play_level_2(Player *p) {
    printf("\n>>> OLD CASTLE <<<\nFind the KEY.\n");
    int map[10]; int boss_count = 0;
    for(int i=0; i<10; i++) { 
        map[i] = (rand() % 6) + 1; 
        if(map[i] == 5) boss_count++; 
    }
    if(boss_count == 0) map[9] = 5; 

    int room = 0, playing = 1;

    while(playing == 1 && p->hp > 0) {
        printf("\n[Level 2] Room: %d/10 | Key: %d | HP: %d\n", room+1, p->has_key, p->hp);
        printf("1. Walk | 2. Store | 3. Run Away (50 money)\nAction: ");
        int act; if(scanf("%d", &act) != 1) { while(getchar() != '\n'); act = 0; }

        if(act == 1) {
            if(room >= 10) { playing = 0; break; }
            Enemy foe = make_enemy_2(map[room]);
            room++;
            printf("--- Enemy: %s ---\n", foe.name);

            if(foe.type == 1) { 
                printf("FIGHT! You need > %d\n", foe.dice_needed);
                while(1) {
                    int roll = (rand()%6)+1;
                    int my_attack = roll + p->has_sword;
                    printf(" > Dice: %d (Total Attack: %d)\n", roll, my_attack);

                    if(my_attack > foe.dice_needed) {
                        printf(" > WIN! +%d money.\n", foe.reward);
                        p->money += foe.reward;
                        if(foe.is_boss == 1) { 
                            printf(" > VAMPIRE DEAD! You got the KEY!\n");
                            p->has_key = 1;
                        }
                        break;
                    }
                    int hurt = foe.damage - p->has_armor; if(hurt<0) hurt=0;
                    printf(" > FAIL! You lost %d HP.\n", hurt);
                    p->hp -= hurt;
                    if(p->hp <= 0) { playing=0; break; }
                }
            } else if(foe.type == 2) { 
                int hurt = foe.damage - p->has_armor; if(hurt<0) hurt=0;
                printf("TRAP! You lost %d HP.\n", hurt);
                p->hp -= hurt;
            }
            if(p->has_key == 1 && p->hp > 0) {
                printf("\n*** LEVEL 2 FINISHED! ***\n");
                p->wins++; playing = 0;
            }
        } else if(act == 2) open_store(p);
        else if(act == 3 && p->money>=50) { p->money-=50; playing=0; }
        else printf("You can't do that!\n");
    }
}

Enemy make_enemy_3(int dice) {
    Enemy e; e.is_boss = 0;
    if (dice == 1) { strcpy(e.name, "Rock Trap"); e.type=2; e.dice_needed=0; e.damage=4; e.reward=0; }
    else if (dice == 2) { strcpy(e.name, "Stone Monster"); e.type=1; e.dice_needed=4; e.damage=3; e.reward=5; }
    else if (dice == 3) { strcpy(e.name, "Dark Knight"); e.type=1; e.dice_needed=5; e.damage=4; e.reward=8; }
    else if (dice == 4) { strcpy(e.name, "Bad Spirit"); e.type=1; e.dice_needed=4; e.damage=2; e.reward=3; }
    else if (dice == 5) { strcpy(e.name, "Lava Trap"); e.type=2; e.dice_needed=0; e.damage=5; e.reward=0; }
    else if (dice == 6) { strcpy(e.name, "DARK BOSS"); e.type=1; e.dice_needed=5; e.damage=6; e.reward=100; e.is_boss=1; } 
    return e;
}

void play_level_3(Player *p) {
    printf("\n>>> DARK CAVE <<<\nKill the DARK BOSS!\n");
    int map[10];
    for(int i=0; i<9; i++) { map[i] = (rand() % 5) + 1; }
    map[9] = 6; 

    int room = 0, playing = 1;

    while(playing == 1 && p->hp > 0) {
        printf("\n[Level 3] Room: %d/10 | FINAL | HP: %d\n", room+1, p->hp);
        printf("1. Walk | 2. Store | 3. Run Away\nAction: ");
        int act; if(scanf("%d", &act) != 1) { while(getchar() != '\n'); act = 0; }

        if(act == 1) {
            Enemy foe = make_enemy_3(map[room]);
            room++;
            printf("--- Enemy: %s ---\n", foe.name);

            if(foe.type == 1) {
                printf("FIGHT! You need > %d\n", foe.dice_needed);
                while(1) {
                    int roll = (rand()%6)+1;
                    int my_attack = roll + p->has_sword;
                    printf(" > Dice: %d (Total Attack: %d)\n", roll, my_attack);

                    if(my_attack > foe.dice_needed) {
                        printf(" > WIN! +%d money.\n", foe.reward);
                        p->money += foe.reward;
                        if(foe.is_boss == 1) { 
                            printf("\n*******************************************\n");
                            printf("!!! YOU KILLED THE DARK BOSS !!!\n");
                            printf("!!!      YOU WON THE GAME!       !!!\n");
                            printf("*******************************************\n");
                            p->wins++;
                            printf("Press Enter to close...");
                            getchar(); getchar();
                            exit(0); 
                        }
                        break;
                    }
                    int hurt = foe.damage - p->has_armor; if(hurt<0) hurt=0;
                    printf(" > FAIL! You lost %d HP.\n", hurt);
                    p->hp -= hurt;
                    if(p->hp <= 0) { 
                        printf("\n!!! YOU DIED! GAME OVER !!!\n");
                        playing=0; 
                    }
                }
            } else if(foe.type == 2) {
                int hurt = foe.damage - p->has_armor; if(hurt<0) hurt=0;
                printf("TRAP! You lost %d HP.\n", hurt);
                p->hp -= hurt;
            }
        } else if(act == 2) open_store(p);
        else if(act == 3 && p->money>=50) { p->money-=50; playing=0; }
        else printf("You can't do that!\n");
    }
}