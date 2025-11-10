#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#define MAX_GAMES 1000
#define MAX_PLAYERS 5000
#define MATCH_HISTORY_SIZE 10000
#define MAX_TEAMS 100
#define MAX_NAME_LENGTH 50
#define FILENAME_GAMES "games.dat"
#define FILENAME_PLAYERS "players.dat"
#define FILENAME_MATCHES "matches.dat"
#define FILENAME_TEAMS "teams.dat"

typedef struct {
    int day;
    int month;
    int year;
} Date;

typedef struct {
    int day;
    int month;
    int year;
    int hour;
    int minute;
} DateTime;

typedef enum {
    FPS,
    MOBA,
    RPG,
    STRATEGY,
    PUZZLE,
    SPORTS,
    RACING,
    SIMULATION,
    ADVENTURE,
    ACTION
} GameType;

typedef struct {
    int id;
    char name[MAX_NAME_LENGTH];
    char publisher[MAX_NAME_LENGTH];
    char developer[MAX_NAME_LENGTH];
    GameType type;
    Date release_date;
    float price;
    int min_age;
    char description[200];
    char platform[30];
    int active;
    int total_sales;
    float average_rating;
} Game;

typedef enum {
    BRONZE,
    SILVER,
    GOLD,
    PLATINUM,
    DIAMOND,
    MASTER
} PlayerLevel;

typedef struct {
    int id;
    char username[MAX_NAME_LENGTH];
    char email[50];
    Date registration_date;
    int total_playtime_hours;
    PlayerLevel level;
    int experience_points;
    int achievements_count;
    int friends_count;
    int favorite_game_id;
    int active;
} Player;

typedef enum {
    CASUAL,
    COMPETITIVE,
    TOURNAMENT,
    CUSTOM,
    PRACTICE
} MatchMode;

typedef struct {
    int id;
    int game_id;
    Date match_date;
    DateTime start_time;
    DateTime end_time;
    MatchMode mode;
    int winner_player_id;
    int loser_player_id;
    int player_count;
    int team1_id;
    int team2_id;
    int duration_minutes;
    int active;
} Match;

typedef struct {
    int id;
    char name[MAX_NAME_LENGTH];
    char tag[10];
    char description[200];
    Date creation_date;
    int member_count;
    int wins;
    int losses;
    int draws;
    int active;
} Team;

typedef struct {
    Game games[MAX_GAMES];
    int game_count;
    Player players[MAX_PLAYERS];
    int player_count;
    Match matches[MATCH_HISTORY_SIZE];
    int match_count;
    Team teams[MAX_TEAMS];
    int team_count;
} GamingDatabase;

// Function prototypes
void initialize_database(GamingDatabase *db);
int add_game(GamingDatabase *db, const char *name, const char *publisher,
               const char *developer, GameType type, Date release_date,
               float price, int min_age, const char *description,
               const char *platform);
int add_player(GamingDatabase *db, const char *username, const char *email,
                 Date registration_date, int favorite_game_id);
int add_team(GamingDatabase *db, const char *name, const char *tag,
              const char *description, Date creation_date);
int create_match(GamingDatabase *db, int game_id, Date match_date,
                 DateTime start_time, DateTime end_time, MatchMode mode,
                 int team1_id, int team2_id);
void end_match(GamingDatabase *db, int match_id, int winner_player_id);
void display_game(const Game *game);
void display_player(const Player *player);
void display_team(const Team *team);
void display_match(const Match *match, const GamingDatabase *db);
void display_all_games(const GamingDatabase *db);
void display_all_players(const GamingDatabase *db);
void display_all_teams(const GamingDatabase *db);
void display_all_matches(const GamingDatabase *db);
void search_games_by_type(const GamingDatabase *db, GameType type);
void search_games_by_price_range(const GamingDatabase *db, float min_price, float max_price);
void display_player_statistics(const GamingDatabase *db, int player_id);
void display_game_statistics(const GamingDatabase *db, int game_id);
void display_team_statistics(const GamingDatabase *db, int team_id);
void generate_player_progress_report(const GamingDatabase *db, int player_id);
void generate_popularity_report(const GamingDatabase *db);
int find_game_by_id(const GamingDatabase *db, int game_id);
int find_player_by_id(const GamingDatabase *db, int player_id);
int find_team_by_id(const GamingDatabase *db, int team_id);
int find_match_by_id(const GamingDatabase *db, int match_id);
int save_database(const GamingDatabase *db);
int load_database(GamingDatabase *db);
void generate_sample_data(GamingDatabase *db);
void add_player_to_team(GamingDatabase *db, int team_id, int player_id);
void remove_player_from_team(GamingDatabase *db, int team_id, int player_id);
void update_player_stats(GamingDatabase *db, int player_id, int experience, int playtime);
int is_valid_date(int day, int month, int year);
int is_valid_time(int hour, int minute);
const char* get_game_type_name(GameType type);
const char* get_player_level_name(PlayerLevel level);
const char* get_match_mode_name(MatchMode mode);

int main() {
    GamingDatabase db;
    initialize_database(&db);
    
    printf("Gaming Platform Management System\n");
    printf("1. Add Game\n");
    printf("2. Add Player\n");
    printf("3. Add Team\n");
    printf("4. Create Match\n");
    printf("5. End Match\n");
    printf("6. Display All Games\n");
    printf("7. Display All Players\n");
    printf("8. Display All Teams\n");
    printf("9. Display All Matches\n");
    printf("10. Search Games by Type\n");
    printf("11. Search Games by Price Range\n");
    printf("12. Display Player Statistics\n");
    printf("13. Display Game Statistics\n");
    printf("14. Display Team Statistics\n");
    printf("15. Generate Player Progress Report\n");
    printf("16. Generate Popularity Report\n");
    printf("17. Save Database\n");
    printf("18. Load Database\n");
    printf("19. Generate Sample Data\n");
    printf("20. Add Player to Team\n");
    printf("21. Remove Player from Team\n");
    printf("22. Update Player Stats\n");
    printf("0. Exit\n");
    
    int choice;
    do {
        printf("\nEnter your choice: ");
        scanf("%d", &choice);
        
        switch (choice) {
            case 1: {
                char name[MAX_NAME_LENGTH], publisher[MAX_NAME_LENGTH];
                char developer[MAX_NAME_LENGTH], description[200], platform[30];
                int type, day, month, year, min_age;
                float price;
                
                printf("Enter game name: ");
                scanf(" %[^\n]", name);
                printf("Enter publisher: ");
                scanf(" %[^\n]", publisher);
                printf("Enter developer: ");
                scanf(" %[^\n]", developer);
                printf("Enter game type (0-9): ");
                scanf("%d", &type);
                printf("Enter release date (DD MM YYYY): ");
                scanf("%d %d %d", &day, &month, &year);
                printf("Enter price: ");
                scanf("%f", &price);
                printf("Enter minimum age: ");
                scanf("%d", &min_age);
                printf("Enter description: ");
                scanf(" %[^\n]", description);
                printf("Enter platform: ");
                scanf(" %[^\n]", platform);
                
                Date release_date = {day, month, year};
                
                int game_id = add_game(&db, name, publisher, developer, (GameType)type,
                                     release_date, price, min_age, description, platform);
                if (game_id != -1) {
                    printf("Game added with ID: %d\n", game_id);
                } else {
                    printf("Failed to add game. Database is full.\n");
                }
                break;
            }
            case 2: {
                char username[MAX_NAME_LENGTH], email[50];
                int day, month, year, favorite_game_id;
                
                printf("Enter username: ");
                scanf(" %[^\n]", username);
                printf("Enter email: ");
                scanf("%s", email);
                printf("Enter registration date (DD MM YYYY): ");
                scanf("%d %d %d", &day, &month, &year);
                printf("Enter favorite game ID (0 for none): ");
                scanf("%d", &favorite_game_id);
                
                Date registration_date = {day, month, year};
                
                int player_id = add_player(&db, username, email, registration_date, favorite_game_id);
                if (player_id != -1) {
                    printf("Player added with ID: %d\n", player_id);
                } else {
                    printf("Failed to add player. Database is full.\n");
                }
                break;
            }
            case 3: {
                char name[MAX_NAME_LENGTH], tag[10], description[200];
                int day, month, year;
                
                printf("Enter team name: ");
                scanf(" %[^\n]", name);
                printf("Enter team tag: ");
                scanf("%s", tag);
                printf("Enter description: ");
                scanf(" %[^\n]", description);
                printf("Enter creation date (DD MM YYYY): ");
                scanf("%d %d %d", &day, &month, &year);
                
                Date creation_date = {day, month, year};
                
                int team_id = add_team(&db, name, tag, description, creation_date);
                if (team_id != -1) {
                    printf("Team added with ID: %d\n", team_id);
                } else {
                    printf("Failed to add team. Database is full.\n");
                }
                break;
            }
            case 4: {
                int game_id, day, month, year, team1_id, team2_id, mode;
                DateTime start_time, end_time;
                
                printf("Enter game ID: ");
                scanf("%d", &game_id);
                printf("Enter match date (DD MM YYYY): ");
                scanf("%d %d %d", &day, &month, &year);
                printf("Enter start time (HH MM): ");
                scanf("%d %d", &start_time.hour, &start_time.minute);
                printf("Enter end time (HH MM): ");
                scanf("%d %d", &end_time.hour, &end_time.minute);
                printf("Enter match mode (0-Casual, 1-Competitive, 2-Tournament, 3-Custom, 4-Practice): ");
                scanf("%d", &mode);
                printf("Enter team 1 ID (0 for no team): ");
                scanf("%d", &team1_id);
                printf("Enter team 2 ID (0 for no team): ");
                scanf("%d", &team2_id);
                
                Date match_date = {day, month, year};
                
                int match_id = create_match(&db, game_id, match_date, start_time, end_time,
                                        (MatchMode)mode, team1_id, team2_id);
                if (match_id != -1) {
                    printf("Match created with ID: %d\n", match_id);
                } else {
                    printf("Failed to create match. Check IDs.\n");
                }
                break;
            }
            case 5: {
                int match_id, winner_player_id;
                
                printf("Enter match ID: ");
                scanf("%d", &match_id);
                printf("Enter winner player ID (0 for draw): ");
                scanf("%d", &winner_player_id);
                
                end_match(&db, match_id, winner_player_id);
                break;
            }
            case 6: {
                display_all_games(&db);
                break;
            }
            case 7: {
                display_all_players(&db);
                break;
            }
            case 8: {
                display_all_teams(&db);
                break;
            }
            case 9: {
                display_all_matches(&db);
                break;
            }
            case 10: {
                int type;
                printf("Enter game type (0-9): ");
                scanf("%d", &type);
                search_games_by_type(&db, (GameType)type);
                break;
            }
            case 11: {
                float min_price, max_price;
                printf("Enter minimum price: ");
                scanf("%f", &min_price);
                printf("Enter maximum price: ");
                scanf("%f", &max_price);
                search_games_by_price_range(&db, min_price, max_price);
                break;
            }
            case 12: {
                int player_id;
                printf("Enter player ID: ");
                scanf("%d", &player_id);
                display_player_statistics(&db, player_id);
                break;
            }
            case 13: {
                int game_id;
                printf("Enter game ID: ");
                scanf("%d", &game_id);
                display_game_statistics(&db, game_id);
                break;
            }
            case 14: {
                int team_id;
                printf("Enter team ID: ");
                scanf("%d", &team_id);
                display_team_statistics(&db, team_id);
                break;
            }
            case 15: {
                int player_id;
                printf("Enter player ID: ");
                scanf("%d", &player_id);
                generate_player_progress_report(&db, player_id);
                break;
            }
            case 16: {
                generate_popularity_report(&db);
                break;
            }
            case 17: {
                if (save_database(&db)) {
                    printf("Database saved successfully.\n");
                } else {
                    printf("Failed to save database.\n");
                }
                break;
            }
            case 18: {
                if (load_database(&db)) {
                    printf("Database loaded successfully.\n");
                } else {
                    printf("Failed to load database.\n");
                }
                break;
            }
            case 19: {
                generate_sample_data(&db);
                printf("Sample data generated.\n");
                break;
            }
            case 20: {
                int team_id, player_id;
                printf("Enter team ID: ");
                scanf("%d", &team_id);
                printf("Enter player ID: ");
                scanf("%d", &player_id);
                add_player_to_team(&db, team_id, player_id);
                break;
            }
            case 21: {
                int team_id, player_id;
                printf("Enter team ID: ");
                scanf("%d", &team_id);
                printf("Enter player ID: ");
                scanf("%d", &player_id);
                remove_player_from_team(&db, team_id, player_id);
                break;
            }
            case 22: {
                int player_id, experience, playtime;
                printf("Enter player ID: ");
                scanf("%d", &player_id);
                printf("Enter experience points to add: ");
                scanf("%d", &experience);
                printf("Enter playtime hours to add: ");
                scanf("%d", &playtime);
                update_player_stats(&db, player_id, experience, playtime);
                break;
            }
            case 0: {
                printf("Exiting program.\n");
                break;
            }
            default: {
                printf("Invalid choice. Please try again.\n");
                break;
            }
        }
    } while (choice != 0);
    
    return 0;
}

void initialize_database(GamingDatabase *db) {
    <｜fim▁hole｜>db->game_count = 0;
    db->player_count = 0;
    db->match_count = 0;
    db->team_count = 0;
}

int add_game(GamingDatabase *db, const char *name, const char *publisher,
               const char *developer, GameType type, Date release_date,
               float price, int min_age, const char *description,
               const char *platform) {
    if (db->game_count >= MAX_GAMES) {
        return -1;
    }
    
    Game *game = &db->games[db->game_count];
    game->id = db->game_count + 1;
    strncpy(game->name, name, MAX_NAME_LENGTH - 1);
    game->name[MAX_NAME_LENGTH - 1] = '\0';
    strncpy(game->publisher, publisher, MAX_NAME_LENGTH - 1);
    game->publisher[MAX_NAME_LENGTH - 1] = '\0';
    strncpy(game->developer, developer, MAX_NAME_LENGTH - 1);
    game->developer[MAX_NAME_LENGTH - 1] = '\0';
    game->type = type;
    game->release_date = release_date;
    game->price = price;
    game->min_age = min_age;
    strncpy(game->description, description, 199);
    game->description[199] = '\0';
    strncpy(game->platform, platform, 29);
    game->platform[29] = '\0';
    game->active = 1;
    game->total_sales = 0;
    game->average_rating = 0.0f;
    
    db->game_count++;
    return game->id;
}

int add_player(GamingDatabase *db, const char *username, const char *email,
                 Date registration_date, int favorite_game_id) {
    if (db->player_count >= MAX_PLAYERS) {
        return -1;
    }
    
    // Validate favorite game exists (if specified)
    if (favorite_game_id != 0 && find_game_by_id(db, favorite_game_id) == -1) {
        printf("Favorite game not found.\n");
        return -1;
    }
    
    Player *player = &db->players[db->player_count];
    player->id = db->player_count + 1;
    strncpy(player->username, username, MAX_NAME_LENGTH - 1);
    player->username[MAX_NAME_LENGTH - 1] = '\0';
    strncpy(player->email, email, 49);
    player->email[49] = '\0';
    player->registration_date = registration_date;
    player->total_playtime_hours = 0;
    player->level = BRONZE;
    player->experience_points = 0;
    player->achievements_count = 0;
    player->friends_count = 0;
    player->favorite_game_id = favorite_game_id;
    player->active = 1;
    
    db->player_count++;
    return player->id;
}

int add_team(GamingDatabase *db, const char *name, const char *tag,
              const char *description, Date creation_date) {
    if (db->team_count >= MAX_TEAMS) {
        return -1;
    }
    
    Team *team = &db->teams[db->team_count];
    team->id = db->team_count + 1;
    strncpy(team->name, name, MAX_NAME_LENGTH - 1);
    team->name[MAX_NAME_LENGTH - 1] = '\0';
    strncpy(team->tag, tag, 9);
    team->tag[9] = '\0';
    strncpy(team->description, description, 199);
    team->description[199] = '\0';
    team->creation_date = creation_date;
    team->member_count = 0;
    team->wins = 0;
    team->losses = 0;
    team->draws = 0;
    team->active = 1;
    
    db->team_count++;
    return team->id;
}

int create_match(GamingDatabase *db, int game_id, Date match_date,
                 DateTime start_time, DateTime end_time, MatchMode mode,
                 int team1_id, int team2_id) {
    if (db->match_count >= MATCH_HISTORY_SIZE) {
        return -1;
    }
    
    // Validate game exists
    if (find_game_by_id(db, game_id) == -1) {
        return -1;
    }
    
    // Validate teams exist (if specified)
    if ((team1_id != 0 && find_team_by_id(db, team1_id) == -1) ||
        (team2_id != 0 && find_team_by_id(db, team2_id) == -1)) {
        return -1;
    }
    
    // Validate date and time
    if (!is_valid_date(match_date.day, match_date.month, match_date.year) ||
        !is_valid_time(start_time.hour, start_time.minute) ||
        !is_valid_time(end_time.hour, end_time.minute)) {
        return -1;
    }
    
    Match *match = &db->matches[db->match_count];
    match->id = db->match_count + 1;
    match->game_id = game_id;
    match->match_date = match_date;
    match->start_time = start_time;
    match->end_time = end_time;
    match->mode = mode;
    match->winner_player_id = 0;
    match->loser_player_id = 0;
    match->team1_id = team1_id;
    match->team2_id = team2_id;
    
    // Calculate duration in minutes
    int start_total_minutes = start_time.hour * 60 + start_time.minute;
    int end_total_minutes = end_time.hour * 60 + end_time.minute;
    match->duration_minutes = end_total_minutes - start_total_minutes;
    if (match->duration_minutes < 0) {
        // If end time is on next day
        match->duration_minutes += 24 * 60;
    }
    
    match->player_count = 0;
    match->active = 1;
    
    db->match_count++;
    return match->id;
}

void end_match(GamingDatabase *db, int match_id, int winner_player_id) {
    int match_index = find_match_by_id(db, match_id);
    if (match_index == -1) {
        printf("Match not found.\n");
        return;
    }
    
    Match *match = &db->matches[match_index];
    
    if (!match->active) {
        printf("Match is already ended.\n");
        return;
    }
    
    // Update match status
    match->active = 0;
    match->winner_player_id = winner_player_id;
    
    printf("Match %d ended. Winner: Player ID %d\n", match_id, winner_player_id);
}

void display_game(const Game *game) {
    printf("\nGame ID: %d\n", game->id);
    printf("Name: %s\n", game->name);
    printf("Publisher: %s\n", game->publisher);
    printf("Developer: %s\n", game->developer);
    printf("Type: %s\n", get_game_type_name(game->type));
    printf("Release Date: %02d/%02d/%d\n", 
           game->release_date.day, game->release_date.month, game->release_date.year);
    printf("Price: $%.2f\n", game->price);
    printf("Minimum Age: %d\n", game->min_age);
    printf("Platform: %s\n", game->platform);
    printf("Description: %s\n", game->description);
    printf("Total Sales: %d\n", game->total_sales);
    printf("Average Rating: %.1f/10\n", game->average_rating);
    printf("Status: %s\n", game->active ? "Active" : "Inactive");
}

void display_player(const Player *player) {
    printf("\nPlayer ID: %d\n", player->id);
    printf("Username: %s\n", player->username);
    printf("Email: %s\n", player->email);
    printf("Registration Date: %02d/%02d/%d\n", 
           player->registration_date.day, player->registration_date.month, player->registration_date.year);
    printf("Total Playtime: %d hours\n", player->total_playtime_hours);
    printf("Level: %s\n", get_player_level_name(player->level));
    printf("Experience Points: %d\n", player->experience_points);
    printf("Achievements: %d\n", player->achievements_count);
    printf("Friends: %d\n", player->friends_count);
    printf("Status: %s\n", player->active ? "Active" : "Inactive");
}

void display_team(const Team *team) {
    printf("\nTeam ID: %d\n", team->id);
    printf("Name: %s\n", team->name);
    printf("Tag: %s\n", team->tag);
    printf("Description: %s\n", team->description);
    printf("Creation Date: %02d/%02d/%d\n", 
           team->creation_date.day, team->creation_date.month, team->creation_date.year);
    printf("Members: %d\n", team->member_count);
    printf("Wins: %d\n", team->wins);
    printf("Losses: %d\n", team->losses);
    printf("Draws: %d\n", team->draws);
    printf("Win Rate: %.1f%%\n", 
           (team->wins + team->losses + team->draws) > 0 ? 
           (float)team->wins / (team->wins + team->losses + team->draws) * 100 : 0.0f);
    printf("Status: %s\n", team->active ? "Active" : "Inactive");
}

void display_match(const Match *match, const GamingDatabase *db) {
    printf("\nMatch ID: %d\n", match->id);
    
    // Display game information
    int game_index = find_game_by_id(db, match->game_id);
    if (game_index != -1) {
        printf("Game: %s\n", db->games[game_index].name);
    }
    
    printf("Date: %02d/%02d/%d\n", 
           match->match_date.day, match->match_date.month, match->match_date.year);
    printf("Start Time: %02d:%02d\n", match->start_time.hour, match->start_time.minute);
    printf("End Time: %02d:%02d\n", match->end_time.hour, match->end_time.minute);
    printf("Duration: %d minutes\n", match->duration_minutes);
    printf("Mode: %s\n", get_match_mode_name(match->mode));
    printf("Players: %d\n", match->player_count);
    
    // Display teams
    if (match->team1_id != 0) {
        int team1_index = find_team_by_id(db, match->team1_id);
        if (team1_index != -1) {
            printf("Team 1: %s (%s)\n", 
                   db->teams[team1_index].name, db->teams[team1_index].tag);
        }
    }
    
    if (match->team2_id != 0) {
        int team2_index = find_team_by_id(db, match->team2_id);
        if (team2_index != -1) {
            printf("Team 2: %s (%s)\n", 
                   db->teams[team2_index].name, db->teams[team2_index].tag);
        }
    }
    
    printf("Winner: %d\n", match->winner_player_id);
    printf("Loser: %d\n", match->loser_player_id);
    printf("Status: %s\n", match->active ? "In Progress" : "Completed");
}

void display_all_games(const GamingDatabase *db) {
    printf("\n=== All Games ===\n");
    printf("Total Games: %d\n\n", db->game_count);
    
    for (int i = 0; i < db->game_count; i++) {
        if (db->games[i].active) {
            display_game(&db->games[i]);
            printf("------------------------\n");
        }
    }
}

void display_all_players(const GamingDatabase *db) {
    printf("\n=== All Players ===\n");
    printf("Total Players: %d\n\n", db->player_count);
    
    for (int i = 0; i < db->player_count; i++) {
        if (db->players[i].active) {
            display_player(&db->players[i]);
            printf("------------------------\n");
        }
    }
}

void display_all_teams(const GamingDatabase *db) {
    printf("\n=== All Teams ===\n");
    printf("Total Teams: %d\n\n", db->team_count);
    
    for (int i = 0; i < db->team_count; i++) {
        if (db->teams[i].active) {
            display_team(&db->teams[i]);
            printf("------------------------\n");
        }
    }
}

void display_all_matches(const GamingDatabase *db) {
    printf("\n=== All Matches ===\n");
    printf("Total Matches: %d\n\n", db->match_count);
    
    for (int i = 0; i < db->match_count; i++) {
        display_match(&db->matches[i], db);
        printf("========================\n");
    }
}

void search_games_by_type(const GamingDatabase *db, GameType type) {
    printf("\n=== Games of Type: %s ===\n", get_game_type_name(type));
    
    int found = 0;
    
    for (int i = 0; i < db->game_count; i++) {
        if (db->games[i].active && db->games[i].type == type) {
            display_game(&db->games[i]);
            printf("------------------------\n");
            found++;
        }
    }
    
    if (found == 0) {
        printf("No games found of this type.\n");
    }
}

void search_games_by_price_range(const GamingDatabase *db, float min_price, float max_price) {
    printf("\n=== Games in Price Range: $%.2f - $%.2f ===\n", min_price, max_price);
    
    int found = 0;
    
    for (int i = 0; i < db->game_count; i++) {
        if (db->games[i].active && 
            db->games[i].price >= min_price && 
            db->games[i].price <= max_price) {
            display_game(&db->games[i]);
            printf("------------------------\n");
            found++;
        }
    }
    
    if (found == 0) {
        printf("No games found in this price range.\n");
    }
}

void display_player_statistics(const GamingDatabase *db, int player_id) {
    int player_index = find_player_by_id(db, player_id);
    if (player_index == -1) {
        printf("Player not found.\n");
        return;
    }
    
    Player *player = &db->players[player_index];
    
    printf("\n=== Player Statistics for %s ===\n", player->username);
    display_player(player);
    
    // Calculate matches played
    int matches_played = 0;
    int matches_won = 0;
    int matches_lost = 0;
    
    for (int i = 0; i < db->match_count; i++) {
        if (db->matches[i].active == 0) { // Only consider completed matches
            // Check if player participated in the match
            // This is a simplified check - in a real system, we'd track player participation more precisely
            if ((rand() % 100) < 20) { // Simulate 20% chance of participation
                matches_played++;
                
                if (db->matches[i].winner_player_id == player_id) {
                    matches_won++;
                } else if (db->matches[i].loser_player_id == player_id) {
                    matches_lost++;
                }
            }
        }
    }
    
    printf("\nMatch Statistics:\n");
    printf("Matches Played: %d\n", matches_played);
    printf("Matches Won: %d\n", matches_won);
    printf("Matches Lost: %d\n", matches_lost);
    printf("Win Rate: %.1f%%\n", 
           matches_played > 0 ? (float)matches_won / matches_played * 100 : 0.0f);
    
    // Calculate total playtime (add a random amount for demonstration)
    int estimated_playtime = player->total_playtime_hours + (matches_played * 30); // 30 minutes per match
    printf("Estimated Total Playtime: %d hours\n", estimated_playtime);
}

void display_game_statistics(const GamingDatabase *db, int game_id) {
    int game_index = find_game_by_id(db, game_id);
    if (game_index == -1) {
        printf("Game not found.\n");
        return;
    }
    
    Game *game = &db->games[game_index];
    
    printf("\n=== Game Statistics for %s ===\n", game->name);
    display_game(game);
    
    // Calculate matches played
    int matches_played = 0;
    
    for (int i = 0; i < db->match_count; i++) {
        if (db->matches[i].game_id == game_id) {
            matches_played++;
        }
    }
    
    printf("\nMatch Statistics:\n");
    printf("Matches Played: %d\n", matches_played);
    
    // Calculate playtime (estimated based on matches)
    int estimated_playtime = matches_played * 30; // 30 minutes per match
    printf("Estimated Total Playtime: %d hours\n", estimated_playtime / 60);
    
    // Calculate popularity
    float popularity = 0.0f;
    int player_count = 0;
    
    for (int i = 0; i < db->player_count; i++) {
        if (db->players[i].favorite_game_id == game_id) {
            player_count++;
        }
    }
    
    popularity = (float)player_count / db->player_count * 100;
    printf("Favorite Game for %d players (%.1f%% of all players)\n", player_count, popularity);
}

void display_team_statistics(const GamingDatabase *db, int team_id) {
    int team_index = find_team_by_id(db, team_id);
    if (team_index == -1) {
        printf("Team not found.\n");
        return;
    }
    
    Team *team = &db->teams[team_index];
    
    printf("\n=== Team Statistics for %s ===\n", team->name);
    display_team(team);
    
    // Calculate matches played
    int matches_played = 0;
    
    for (int i = 0; i < db->match_count; i++) {
        if ((db->matches[i].team1_id == team_id || db->matches[i].team2_id == team_id) &&
            db->matches[i].active == 0) { // Only consider completed matches
            matches_played++;
        }
    }
    
    printf("\nMatch Statistics:\n");
    printf("Matches Played: %d\n", matches_played);
    
    // Calculate team performance
    int total_matches = team->wins + team->losses + team->draws;
    printf("Performance: %d wins, %d losses, %d draws\n", 
           team->wins, team->losses, team->draws);
    printf("Win Rate: %.1f%%\n", 
           total_matches > 0 ? (float)team->wins / total_matches * 100 : 0.0f);
}

void generate_player_progress_report(const GamingDatabase *db, int player_id) {
    int player_index = find_player_by_id(db, player_id);
    if (player_index == -1) {
        printf("Player not found.\n");
        return;
    }
    
    Player *player = &db->players[player_index];
    
    printf("\n=== Progress Report for %s ===\n", player->username);
    printf("Player ID: %d\n", player->id);
    printf("Current Level: %s\n", get_player_level_name(player->level));
    printf("Experience Points: %d\n", player->experience_points);
    printf("Total Playtime: %d hours\n", player->total_playtime_hours);
    printf("Achievements: %d\n", player->achievements_count);
    printf("Friends: %d\n", player->friends_count);
    
    // Calculate next level requirements
    int next_level_points = 0;
    const char* next_level_name = "";
    
    switch (player->level) {
        case BRONZE:
            next_level_points = 100;
            next_level_name = "Silver";
            break;
        case SILVER:
            next_level_points = 300;
            next_level_name = "Gold";
            break;
        case GOLD:
            next_level_points = 1000;
            next_level_name = "Platinum";
            break;
        case PLATINUM:
            next_level_points = 3000;
            next_level_name = "Diamond";
            break;
        case DIAMOND:
            next_level_points = 10000;
            next_level_name = "Master";
            break;
        case MASTER:
            next_level_points = -1; // Already at max level
            next_level_name = "N/A";
            break;
    }
    
    if (next_level_points > 0) {
        int points_needed = next_level_points - player->experience_points;
        printf("\nNext Level (%s):\n", next_level_name);
        printf("Experience Points Needed: %d\n", points_needed);
        
        // Estimate playtime needed (assuming 10 points per hour of gameplay)
        int hours_needed = points_needed / 10;
        printf("Estimated Playtime Needed: %d hours\n", hours_needed);
    } else {
        printf("\nCongratulations! You've reached the maximum level.\n");
    }
}

void generate_popularity_report(const GamingDatabase *db) {
    printf("\n=== Platform Popularity Report ===\n");
    
    printf("Total Games: %d\n", db->game_count);
    printf("Total Players: %d\n", db->player_count);
    printf("Total Teams: %d\n", db->team_count);
    printf("Total Matches: %d\n", db->match_count);
    
    // Game type distribution
    int type_counts[10] = {0}; // FPS to ACTION
    
    for (int i = 0; i < db->game_count; i++) {
        if (db->games[i].active) {
            type_counts[db->games[i].type]++;
        }
    }
    
    printf("\nGame Type Distribution:\n");
    for (int i = 0; i < 10; i++) {
        if (type_counts[i] > 0) {
            printf("  %s: %d (%.1f%%)\n", 
                   get_game_type_name((GameType)i), type_counts[i],
                   db->game_count > 0 ? (float)type_counts[i] / db->game_count * 100 : 0);
        }
    }
    
    // Player level distribution
    int level_counts[6] = {0}; // BRONZE to MASTER
    
    for (int i = 0; i < db->player_count; i++) {
        if (db->players[i].active) {
            level_counts[db->players[i].level]++;
        }
    }
    
    printf("\nPlayer Level Distribution:\n");
    for (int i = 0; i < 6; i++) {
        if (level_counts[i] > 0) {
            printf("  %s: %d (%.1f%%)\n", 
                   get_player_level_name((PlayerLevel)i), level_counts[i],
                   db->player_count > 0 ? (float)level_counts[i] / db->player_count * 100 : 0);
        }
    }
    
    // Most popular games
    int favorite_counts[MAX_GAMES] = {0};
    
    for (int i = 0; i < db->player_count; i++) {
        if (db->players[i].active && db->players[i].favorite_game_id > 0) {
            favorite_counts[db->players[i].favorite_game_id - 1]++;
        }
    }
    
    // Sort games by popularity
    int sorted_indices[MAX_GAMES];
    for (int i = 0; i < db->game_count; i++) {
        sorted_indices[i] = i;
    }
    
    for (int i = 0; i < db->game_count - 1; i++) {
        for (int j = 0; j < db->game_count - i - 1; j++) {
            if (favorite_counts[sorted_indices[j]] < favorite_counts[sorted_indices[j + 1]]) {
                int temp = sorted_indices[j];
                sorted_indices[j] = sorted_indices[j + 1];
                sorted_indices[j + 1] = temp;
            }
        }
    }
    
    printf("\nTop 10 Most Popular Games:\n");
    printf("%-5s %-30s %-10s\n", "ID", "Name", "Favorites");
    printf("------------------------------------------\n");
    
    int display_count = db->game_count < 10 ? db->game_count : 10;
    for (int i = 0; i < display_count; i++) {
        int game_index = sorted_indices[db->game_count - i - 1];
        printf("%-5d %-30s %-10d\n", 
               db->games[game_index].id, 
               db->games[game_index].name, 
               favorite_counts[game_index]);
    }
    
    // Most active teams
    printf("\nTop 10 Most Active Teams:\n");
    printf("%-5s %-30s %-10s %-10s\n", "ID", "Name", "Members", "Matches");
    printf("----------------------------------------------\n");
    
    int team_matches[MAX_TEAMS] = {0};
    
    for (int i = 0; i < db->team_count; i++) {
        if (db->teams[i].active) {
            for (int j = 0; j < db->match_count; j++) {
                if ((db->matches[j].team1_id == db->teams[i].id || 
                    db->matches[j].team2_id == db->teams[i].id) &&
                    db->matches[j].active == 0) { // Only count completed matches
                    team_matches[i]++;
                }
            }
        }
    }
    
    // Sort teams by matches played
    int sorted_team_indices[MAX_TEAMS];
    for (int i = 0; i < db->team_count; i++) {
        sorted_team_indices[i] = i;
    }
    
    for (int i = 0; i < db->team_count - 1; i++) {
        for (int j = 0; j < db->team_count - i - 1; j++) {
            if (team_matches[sorted_team_indices[j]] < team_matches[sorted_team_indices[j + 1]]) {
                int temp = sorted_team_indices[j];
                sorted_team_indices[j] = sorted_team_indices[j + 1];
                sorted_team_indices[j + 1] = temp;
            }
        }
    }
    
    display_count = db->team_count < 10 ? db->team_count : 10;
    for (int i = 0; i < display_count; i++) {
        int team_index = sorted_team_indices[db->team_count - i - 1];
        printf("%-5d %-30s %-10d %-10d\n", 
               db->teams[team_index].id, 
               db->teams[team_index].name, 
               db->teams[team_index].member_count, 
               team_matches[team_index]);
    }
}

int find_game_by_id(const GamingDatabase *db, int game_id) {
    for (int i = 0; i < db->game_count; i++) {
        if (db->games[i].id == game_id && db->games[i].active) {
            return i;
        }
    }
    return -1;
}

int find_player_by_id(const GamingDatabase *db, int player_id) {
    for (int i = 0; i < db->player_count; i++) {
        if (db->players[i].id == player_id && db->players[i].active) {
            return i;
        }
    }
    return -1;
}

int find_team_by_id(const GamingDatabase *db, int team_id) {
    for (int i = 0; i < db->team_count; i++) {
        if (db->teams[i].id == team_id && db->teams[i].active) {
            return i;
        }
    }
    return -1;
}

int find_match_by_id(const GamingDatabase *db, int match_id) {
    for (int i = 0; i < db->match_count; i++) {
        if (db->matches[i].id == match_id) {
            return i;
        }
    }
    return -1;
}

int save_database(const GamingDatabase *db) {
    FILE *games_file = fopen(FILENAME_GAMES, "wb");
    if (games_file != NULL) {
        fwrite(&db->game_count, sizeof(int), 1, games_file);
        fwrite(db->games, sizeof(Game), db->game_count, games_file);
        fclose(games_file);
    } else {
        return 0;
    }
    
    FILE *players_file = fopen(FILENAME_PLAYERS, "wb");
    if (players_file != NULL) {
        fwrite(&db->player_count, sizeof(int), 1, players_file);
        fwrite(db->players, sizeof(Player), db->player_count, players_file);
        fclose(players_file);
    } else {
        return 0;
    }
    
    FILE *teams_file = fopen(FILENAME_TEAMS, "wb");
    if (teams_file != NULL) {
        fwrite(&db->team_count, sizeof(int), 1, teams_file);
        fwrite(db->teams, sizeof(Team), db->team_count, teams_file);
        fclose(teams_file);
    } else {
        return 0;
    }
    
    FILE *matches_file = fopen(FILENAME_MATCHES, "wb");
    if (matches_file != NULL) {
        fwrite(&db->match_count, sizeof(int), 1, matches_file);
        fwrite(db->matches, sizeof(Match), db->match_count, matches_file);
        fclose(matches_file);
        return 1;
    }
    
    return 0;
}

int load_database(GamingDatabase *db) {
    FILE *games_file = fopen(FILENAME_GAMES, "rb");
    if (games_file != NULL) {
        fread(&db->game_count, sizeof(int), 1, games_file);
        fread(db->games, sizeof(Game), db->game_count, games_file);
        fclose(games_file);
    } else {
        return 0;
    }
    
    FILE *players_file = fopen(FILENAME_PLAYERS, "rb");
    if (players_file != NULL) {
        fread(&db->player_count, sizeof(int), 1, players_file);
        fread(db->players, sizeof(Player), db->player_count, players_file);
        fclose(players_file);
    } else {
        return 0;
    }
    
    FILE *teams_file = fopen(FILENAME_TEAMS, "rb");
    if (teams_file != NULL) {
        fread(&db->team_count, sizeof(int), 1, teams_file);
        fread(db->teams, sizeof(Team), db->team_count, teams_file);
        fclose(teams_file);
    } else {
        return 0;
    }
    
    FILE *matches_file = fopen(FILENAME_MATCHES, "rb");
    if (matches_file != NULL) {
        fread(&db->match_count, sizeof(int), 1, matches_file);
        fread(db->matches, sizeof(Match), db->match_count, matches_file);
        fclose(matches_file);
        return 1;
    }
    
    return 0;
}

void generate_sample_data(GamingDatabase *db) {
    char* game_names[] = {
        "Battlefield 2042", "Cyberpunk Revolution", "Elder Scrolls VI",
        "Call of Duty: Future Warfare", "FIFA 2025", "Minecraft Legends",
        "Grand Theft Auto VII", "Pokemon Ultra", "Zelda: The New Adventure",
        "Super Mario Universe", "Halo Infinite", "Fortnite: Chapter 4",
        "Apex Legends: Evolution", "League of Legends: New Champions",
        "Valorant: Global Offensive", "Dota 3", "World of Warcraft: Next Gen",
        "Starfield Explorer", "The Sims 5", "Rocket League 2"
    };
    
    char* publishers[] = {
        "Electronic Arts", "Ubisoft", "Bethesda", "Activision",
        "2K Games", "Nintendo", "Sony", "Microsoft", "Valve", "Riot Games"
    };
    
    char* developers[] = {
        "DICE", "CD Projekt Red", "Bethesda Game Studios", "Infinity Ward",
        "EA Canada", "Mojang Studios", "Rockstar Games", "Game Freak", "Nintendo EAD",
        "343 Industries", "Epic Games", "Respawn Entertainment", "Riot Games",
        "Valve Corporation", "Valve Corporation", "Blizzard Entertainment",
        "Bethesda Game Studios", "Maxis", "Psyonix"
    };
    
    char* platforms[] = {
        "PC", "PlayStation 5", "Xbox Series X", "Nintendo Switch",
        "Mobile", "Steam Deck", "Meta Quest 3", "Cloud Gaming"
    };
    
    char* usernames[] = {
        "DragonSlayer", "NinjaWarrior", "PhoenixRising", "ShadowHunter",
        "ThunderBolt", "IceQueen", "FireStorm", "EarthShaker",
        "WindWalker", "WaterMaster", "MysticMage", "IronFist",
        "SilverArrow", "GoldenShield", "DarkKnight", "LightBringer"
    };
    
    char* team_names[] = {
        "Elite Squad", "Thunder Team", "Phoenix Clan", "Shadow Guild",
        "Dragon Alliance", "Storm Brigade", "Fire Battalion", "Ice Legion",
        "Wind Company", "Water Division", "Earth Corps", "Light Force",
        "Dark Empire", "Silver Knights", "Gold Warriors", "Bronze Guardians"
    };
    
    char* team_tags[] = {
        "ELT", "THNDR", "PHNX", "SHDW", "DRGN", "STORM",
        "FIRE", "ICE", "WIND", "WTR", "EARTH", "LGHT",
        "DRK", "SLVR", "GLD", "BRNZ"
    };
    
    srand(time(NULL));
    
    // Add games
    for (int i = 0; i < 30 && db->game_count < MAX_GAMES; i++) {
        char description[200];
        sprintf(description, "Exciting %s game with amazing graphics and gameplay", 
                (i % 2 == 0) ? "action" : "adventure");
        
        int day = 1 + (rand() % 28);
        int month = 1 + (rand() % 12);
        int year = 2015 + (rand() % 8);
        Date release_date = {day, month, year};
        
        float price = 10.0f + (rand() % 60); // $10 to $70
        int min_age = (rand() % 2 == 0) ? 0 : (12 + (rand() % 6)); // 0 or 12-17
        GameType type = (GameType)(rand() % 10);
        const char *platform = platforms[rand() % 8];
        const char *publisher = publishers[rand() % 10];
        const char *developer = developers[rand() % 10];
        
        add_game(db, game_names[i], publisher, developer, type, release_date,
                 price, min_age, description, platform);
    }
    
    // Add players
    for (int i = 0; i < 100 && db->player_count < MAX_PLAYERS; i++) {
        char email[50];
        sprintf(email, "player%d@example.com", i + 1);
        
        int day = 1 + (rand() % 28);
        int month = 1 + (rand() % 12);
        int year = 2015 + (rand() % 8);
        Date registration_date = {day, month, year};
        
        int favorite_game_id = 1 + (rand() % db->game_count);
        
        int player_id = add_player(db, usernames[i % 16], email, registration_date, favorite_game_id);
        
        if (player_id != -1) {
            // Set initial playtime and experience
            db->players[player_id - 1].total_playtime_hours = rand() % 500;
            db->players[player_id - 1].experience_points = rand() % 1000;
            
            // Set level based on experience
            int exp = db->players[player_id - 1].experience_points;
            if (exp >= 10000) {
                db->players[player_id - 1].level = MASTER;
            } else if (exp >= 3000) {
                db->players[player_id - 1].level = DIAMOND;
            } else if (exp >= 1000) {
                db->players[player_id - 1].level = PLATINUM;
            } else if (exp >= 300) {
                db->players[player_id - 1].level = GOLD;
            } else if (exp >= 100) {
                db->players[player_id - 1].level = SILVER;
            } else {
                db->players[player_id - 1].level = BRONZE;
            }
            
            // Set random achievements and friends
            db->players[player_id - 1].achievements_count = rand() % 50;
            db->players[player_id - 1].friends_count = rand() % 100;
        }
    }
    
    // Add teams
    for (int i = 0; i < 20 && db->team_count < MAX_TEAMS; i++) {
        char description[200];
        sprintf(description, "Competitive %s team dedicated to winning tournaments", 
                (i % 2 == 0) ? "esports" : "casual");
        
        int day = 1 + (rand() % 28);
        int month = 1 + (rand() % 12);
        int year = 2015 + (rand() % 8);
        Date creation_date = {day, month, year};
        
        add_team(db, team_names[i], team_tags[i], description, creation_date);
    }
    
    // Create matches
    for (int i = 0; i < 200 && db->match_count < MATCH_HISTORY_SIZE; i++) {
        int game_id = 1 + (rand() % db->game_count);
        int team1_id = 1 + (rand() % db->team_count);
        int team2_id = 1 + (rand() % db->team_count);
        MatchMode mode = (MatchMode)(rand() % 5);
        
        // Random date within last 30 days
        int day_offset = rand() % 30;
        DateTime start_time;
        
        time_t now = time(NULL);
        struct tm *current_time = localtime(&now);
        
        start_time.day = current_time->tm_mday - day_offset;
        start_time.month = current_time->tm_mon + 1;
        start_time.year = current_time->tm_year + 1900;
        start_time.hour = 10 + (rand() % 12); // 10 AM to 10 PM
        start_time.minute = (rand() % 4) * 15; // 0, 15, 30, or 45 minutes
        
        // Normalize date if days go below 1
        if (start_time.day < 1) {
            start_time.day += 28;
            start_time.month--;
            if (start_time.month < 1) {
                start_time.month += 12;
                start_time.year--;
            }
        }
        
        DateTime end_time = start_time;
        int duration = 15 + (rand() % 105); // 15 to 120 minutes
        end_time.minute += duration;
        
        // Normalize time
        while (end_time.minute >= 60) {
            end_time.minute -= 60;
            end_time.hour++;
        }
        
        while (end_time.hour >= 24) {
            end_time.hour -= 24;
            end_time.day++;
            
            if (end_time.day > 28) {
                end_time.day -= 28;
                end_time.month++;
                if (end_time.month > 12) {
                    end_time.month -= 12;
                    end_time.year++;
                }
            }
        }
        
        Date match_date = {start_time.day, start_time.month, start_time.year};
        
        int match_id = create_match(db, game_id, match_date, start_time, end_time,
                                mode, team1_id, team2_id);
        
        if (match_id != -1) {
            // End some matches
            if (rand() % 10 < 7) { // 70% chance
                int winner = (rand() % 2 == 0) ? 1 : 2;
                end_match(db, match_id, winner);
            }
        }
    }
    
    // Add players to teams
    for (int i = 0; i < db->player_count && i < 100; i++) {
        if (db->players[i].active) {
            int team_id = 1 + (rand() % db->team_count);
            add_player_to_team(db, team_id, db->players[i].id);
        }
    }
}

void add_player_to_team(GamingDatabase *db, int team_id, int player_id) {
    int team_index = find_team_by_id(db, team_id);
    int player_index = find_player_by_id(db, player_id);
    
    if (team_index == -1) {
        printf("Team not found.\n");
        return;
    }
    
    if (player_index == -1) {
        printf("Player not found.\n");
        return;
    }
    
    Team *team = &db->teams[team_index];
    team->member_count++;
    
    printf("Player %s added to team %s.\n", 
           db->players[player_index].username, team->name);
}

void remove_player_from_team(GamingDatabase *db, int team_id, int player_id) {
    int team_index = find_team_by_id(db, team_id);
    int player_index = find_player_by_id(db, player_id);
    
    if (team_index == -1) {
        printf("Team not found.\n");
        return;
    }
    
    if (player_index == -1) {
        printf("Player not found.\n");
        return;
    }
    
    Team *team = &db->teams[team_index];
    
    if (team->member_count > 0) {
        team->member_count--;
        printf("Player %s removed from team %s.\n", 
               db->players[player_index].username, team->name);
    } else {
        printf("Team is already empty.\n");
    }
}

void update_player_stats(GamingDatabase *db, int player_id, int experience, int playtime) {
    int player_index = find_player_by_id(db, player_id);
    if (player_index == -1) {
        printf("Player not found.\n");
        return;
    }
    
    Player *player = &db->players[player_index];
    
    int old_experience = player->experience_points;
    int old_playtime = player->total_playtime_hours;
    PlayerLevel old_level = player->level;
    
    player->experience_points += experience;
    player->total_playtime_hours += playtime;
    
    // Update level based on new experience
    int exp = player->experience_points;
    if (exp >= 10000) {
        player->level = MASTER;
    } else if (exp >= 3000) {
        player->level = DIAMOND;
    } else if (exp >= 1000) {
        player->level = PLATINUM;
    } else if (exp >= 300) {
        player->level = GOLD;
    } else if (exp >= 100) {
        player->level = SILVER;
    }
    
    printf("Player %s stats updated:\n", player->username);
    printf("Experience: %d → %d\n", old_experience, player->experience_points);
    printf("Playtime: %d → %d hours\n", old_playtime, player->total_playtime_hours);
    printf("Level: %s → %s\n", 
           get_player_level_name(old_level), get_player_level_name(player->level));
}

int is_valid_date(int day, int month, int year) {
    if (year < 1900 || year > 2100) return 0;
    if (month < 1 || month > 12) return 0;
    
    int days_in_month[] = {31, 28, 31, 30, 31, 30, 31, 30, 31, 30, 31};
    
    // Check for leap year
    if (month == 2 && ((year % 400 == 0) || (year % 100 != 0 && year % 4 == 0))) {
        days_in_month[1] = 29;
    }
    
    if (day < 1 || day > days_in_month[month - 1]) return 0;
    
    return 1;
}

int is_valid_time(int hour, int minute) {
    if (hour < 0 || hour > 23) return 0;
    if (minute < 0 || minute > 59) return 0;
    return 1;
}

const char* get_game_type_name(GameType type) {
    switch (type) {
        case FPS: return "FPS";
        case MOBA: return "MOBA";
        case RPG: return "RPG";
        case STRATEGY: return "Strategy";
        case PUZZLE: return "Puzzle";
        case SPORTS: return "Sports";
        case RACING: return "Racing";
        case SIMULATION: return "Simulation";
        case ADVENTURE: return "Adventure";
        case ACTION: return "Action";
        default: return "Unknown";
    }
}

const char* get_player_level_name(PlayerLevel level) {
    switch (level) {
        case BRONZE: return "Bronze";
        case SILVER: return "Silver";
        case GOLD: return "Gold";
        case PLATINUM: return "Platinum";
        case DIAMOND: return "Diamond";
        case MASTER: return "Master";
        default: return "Unknown";
    }
}

const char* get_match_mode_name(MatchMode mode) {
    switch (mode) {
        case CASUAL: return "Casual";
        case COMPETITIVE: return "Competitive";
        case TOURNAMENT: return "Tournament";
        case CUSTOM: return "Custom";
        case PRACTICE: return "Practice";
        default: return "Unknown";
    }
}