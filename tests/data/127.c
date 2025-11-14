#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#define MAX_PLAYERS 1000
#define MAX_TEAMS 50
#define MAX_GAMES 500
#define MAX_LEAGUES 10
#define MAX_NAME_LENGTH 50
#define FILENAME_PLAYERS "players.dat"
#define FILENAME_TEAMS "teams.dat"
#define FILENAME_GAMES "games.dat"
#define FILENAME_LEAGUES "leagues.dat"

typedef struct {
    int day;
    int month;
    int year;
} Date;

typedef struct {
    int id;
    char first_name[MAX_NAME_LENGTH];
    char last_name[MAX_NAME_LENGTH];
    char position[20]; // "Forward", "Midfielder", "Defender", "Goalkeeper"
    int age;
    double height; // in meters
    double weight; // in kg
    int team_id;
    int jersey_number;
    int active;
    int goals_scored;
    int assists;
    int yellow_cards;
    int red_cards;
    int games_played;
    double market_value; // in millions
    Date contract_expiration;
    char nationality[50];
} Player;

typedef struct {
    int id;
    char name[MAX_NAME_LENGTH];
    char city[50];
    char stadium[MAX_NAME_LENGTH];
    int stadium_capacity;
    char manager_name[MAX_NAME_LENGTH];
    int league_id;
    char colors[50];
    int founded_year;
    int active;
    int points;
    int games_played;
    int wins;
    int draws;
    int losses;
    int goals_for;
    int goals_against;
    double budget; // in millions
} Team;

typedef struct {
    int id;
    int home_team_id;
    int away_team_id;
    Date date;
    char time[10]; // HH:MM
    int home_score;
    int away_score;
    int league_id;
    int week_number;
    int finished;
    char venue[MAX_NAME_LENGTH];
    int attendance;
} Game;

typedef struct {
    int id;
    char name[MAX_NAME_LENGTH];
    char country[50];
    char description[200];
    int teams_count;
    int games_per_team;
    int active;
    int start_month;
    int end_month;
} League;

typedef struct {
    Player players[MAX_PLAYERS];
    int player_count;
    Team teams[MAX_TEAMS];
    int team_count;
    Game games[MAX_GAMES];
    int game_count;
    League leagues[MAX_LEAGUES];
    int league_count;
} FootballDatabase;

// Function prototypes
void initialize_database(FootballDatabase *db);
int add_player(FootballDatabase *db, const char *first_name, const char *last_name,
               const char *position, int age, double height, double weight,
               int team_id, int jersey_number, const char *nationality, double market_value,
               int contract_exp_day, int contract_exp_month, int contract_exp_year);
int add_team(FootballDatabase *db, const char *name, const char *city, const char *stadium,
             int stadium_capacity, const char *manager_name, int league_id, 
             const char *colors, int founded_year, double budget);
int add_league(FootballDatabase *db, const char *name, const char *country,
               const char *description, int teams_count, int games_per_team,
               int start_month, int end_month);
int add_game(FootballDatabase *db, int home_team_id, int away_team_id, 
             int day, int month, int year, const char *time, int league_id, 
             int week_number, const char *venue);
void update_game_score(FootballDatabase *db, int game_id, int home_score, int away_score);
void transfer_player(FootballDatabase *db, int player_id, int new_team_id, double transfer_fee);
void update_team_stats(FootballDatabase *db);
void display_player(const Player *player, const FootballDatabase *db);
void display_team(const Team *team, const FootballDatabase *db);
void display_game(const Game *game, const FootballDatabase *db);
void display_league(const League *league);
void display_all_players(const FootballDatabase *db);
void display_all_teams(const FootballDatabase *db);
void display_all_games(const FootballDatabase *db);
void display_all_leagues(const FootballDatabase *db);
void search_players_by_name(const FootballDatabase *db, const char *name);
void search_teams_by_city(const FootballDatabase *db, const char *city);
void search_games_by_team(const FootballDatabase *db, int team_id);
void generate_player_report(const FootballDatabase *db, int player_id);
void generate_team_report(const FootballDatabase *db, int team_id);
void generate_league_table(const FootballDatabase *db, int league_id);
void generate_top_scorers(const FootballDatabase *db, int league_id);
void generate_fixtures(const FootballDatabase *db, int league_id);
int find_player_by_id(const FootballDatabase *db, int player_id);
int find_team_by_id(const FootballDatabase *db, int team_id);
int find_game_by_id(const FootballDatabase *db, int game_id);
int find_league_by_id(const FootballDatabase *db, int league_id);
int save_database(const FootballDatabase *db);
int load_database(FootballDatabase *db);
void generate_sample_data(FootballDatabase *db);
void simulate_games(FootballDatabase *db, int league_id, int num_games);
void update_player_stats(FootballDatabase *db, int player_id, int goals, int assists, 
                         int yellow_cards, int red_cards);
const char* get_position_description(const char *position);

int main() {
    FootballDatabase db;
    initialize_database(&db);
    
    printf("Football Management System\n");
    printf("1. Add Player\n");
    printf("2. Add Team\n");
    printf("3. Add League\n");
    printf("4. Add Game\n");
    printf("5. Update Game Score\n");
    printf("6. Transfer Player\n");
    printf("7. Update Team Stats\n");
    printf("8. Display All Players\n");
    printf("9. Display All Teams\n");
    printf("10. Display All Games\n");
    printf("11. Display All Leagues\n");
    printf("12. Search Players by Name\n");
    printf("13. Search Teams by City\n");
    printf("14. Search Games by Team\n");
    printf("15. Generate Player Report\n");
    printf("16. Generate Team Report\n");
    printf("17. Generate League Table\n");
    printf("18. Generate Top Scorers\n");
    printf("19. Generate Fixtures\n");
    printf("20. Simulate Games\n");
    printf("21. Update Player Stats\n");
    printf("22. Save Database\n");
    printf("23. Load Database\n");
    printf("24. Generate Sample Data\n");
    printf("0. Exit\n");
    
    int choice;
    do {
        printf("\nEnter your choice: ");
        scanf("%d", &choice);
        
        switch (choice) {
            case 1: {
                char first_name[MAX_NAME_LENGTH], last_name[MAX_NAME_LENGTH];
                char position[20], nationality[50];
                int age, team_id, jersey_number;
                double height, weight, market_value;
                int contract_exp_day, contract_exp_month, contract_exp_year;
                
                printf("Enter first name: ");
                scanf(" %[^\n]", first_name);
                printf("Enter last name: ");
                scanf(" %[^\n]", last_name);
                printf("Enter position (Forward, Midfielder, Defender, Goalkeeper): ");
                scanf(" %[^\n]", position);
                printf("Enter age: ");
                scanf("%d", &age);
                printf("Enter height (m): ");
                scanf("%lf", &height);
                printf("Enter weight (kg): ");
                scanf("%lf", &weight);
                printf("Enter team ID: ");
                scanf("%d", &team_id);
                printf("Enter jersey number: ");
                scanf("%d", &jersey_number);
                printf("Enter nationality: ");
                scanf(" %[^\n]", nationality);
                printf("Enter market value (millions): ");
                scanf("%lf", &market_value);
                printf("Enter contract expiration (DD MM YYYY): ");
                scanf("%d %d %d", &contract_exp_day, &contract_exp_month, &contract_exp_year);
                
                if (strcmp(position, "Forward") != 0 && strcmp(position, "Midfielder") != 0 &&
                    strcmp(position, "Defender") != 0 && strcmp(position, "Goalkeeper") != 0) {
                    printf("Invalid position. Must be one of: Forward, Midfielder, Defender, Goalkeeper.\n");
                    break;
                }
                
                int player_id = add_player(&db, first_name, last_name, position, age, height, weight,
                                         team_id, jersey_number, nationality, market_value,
                                         contract_exp_day, contract_exp_month, contract_exp_year);
                if (player_id != -1) {
                    printf("Player added with ID: %d\n", player_id);
                } else {
                    printf("Failed to add player. Database is full or invalid team ID.\n");
                }
                break;
            }
            case 2: {
                char name[MAX_NAME_LENGTH], city[50], stadium[MAX_NAME_LENGTH];
                char manager_name[MAX_NAME_LENGTH], colors[50];
                int stadium_capacity, league_id, founded_year;
                double budget;
                
                printf("Enter team name: ");
                scanf(" %[^\n]", name);
                printf("Enter city: ");
                scanf(" %[^\n]", city);
                printf("Enter stadium name: ");
                scanf(" %[^\n]", stadium);
                printf("Enter stadium capacity: ");
                scanf("%d", &stadium_capacity);
                printf("Enter manager name: ");
                scanf(" %[^\n]", manager_name);
                printf("Enter league ID: ");
                scanf("%d", &league_id);
                printf("Enter team colors: ");
                scanf(" %[^\n]", colors);
                printf("Enter founded year: ");
                scanf("%d", &founded_year);
                printf("Enter budget (millions): ");
                scanf("%lf", &budget);
                
                int team_id = add_team(&db, name, city, stadium, stadium_capacity, manager_name,
                                     league_id, colors, founded_year, budget);
                if (team_id != -1) {
                    printf("Team added with ID: %d\n", team_id);
                } else {
                    printf("Failed to add team. Database is full or invalid league ID.\n");
                }
                break;
            }
            case 3: {
                char name[MAX_NAME_LENGTH], country[50], description[200];
                int teams_count, games_per_team, start_month, end_month;
                
                printf("Enter league name: ");
                scanf(" %[^\n]", name);
                printf("Enter country: ");
                scanf(" %[^\n]", country);
                printf("Enter description: ");
                scanf(" %[^\n]", description);
                printf("Enter number of teams: ");
                scanf("%d", &teams_count);
                printf("Enter games per team: ");
                scanf("%d", &games_per_team);
                printf("Enter start month (1-12): ");
                scanf("%d", &start_month);
                printf("Enter end month (1-12): ");
                scanf("%d", &end_month);
                
                int league_id = add_league(&db, name, country, description, teams_count,
                                         games_per_team, start_month, end_month);
                if (league_id != -1) {
                    printf("League added with ID: %d\n", league_id);
                } else {
                    printf("Failed to add league. Database is full.\n");
                }
                break;
            }
            case 4: {
                int home_team_id, away_team_id, league_id, week_number;
                int day, month, year;
                char time[10], venue[MAX_NAME_LENGTH];
                
                printf("Enter home team ID: ");
                scanf("%d", &home_team_id);
                printf("Enter away team ID: ");
                scanf("%d", &away_team_id);
                printf("Enter date (DD MM YYYY): ");
                scanf("%d %d %d", &day, &month, &year);
                printf("Enter time (HH:MM): ");
                scanf("%s", time);
                printf("Enter league ID: ");
                scanf("%d", &league_id);
                printf("Enter week number: ");
                scanf("%d", &week_number);
                printf("Enter venue: ");
                scanf(" %[^\n]", venue);
                
                int game_id = add_game(&db, home_team_id, away_team_id, day, month, year,
                                     time, league_id, week_number, venue);
                if (game_id != -1) {
                    printf("Game added with ID: %d\n", game_id);
                } else {
                    printf("Failed to add game. Database is full or invalid IDs.\n");
                }
                break;
            }
            case 5: {
                int game_id, home_score, away_score;
                
                printf("Enter game ID: ");
                scanf("%d", &game_id);
                printf("Enter home score: ");
                scanf("%d", &home_score);
                printf("Enter away score: ");
                scanf("%d", &away_score);
                
                update_game_score(&db, game_id, home_score, away_score);
                break;
            }
            case 6: {
                int player_id, new_team_id;
                double transfer_fee;
                
                printf("Enter player ID: ");
                scanf("%d", &player_id);
                printf("Enter new team ID: ");
                scanf("%d", &new_team_id);
                printf("Enter transfer fee (millions): ");
                scanf("%lf", &transfer_fee);
                
                transfer_player(&db, player_id, new_team_id, transfer_fee);
                break;
            }
            case 7: {
                update_team_stats(&db);
                break;
            }
            case 8: {
                display_all_players(&db);
                break;
            }
            case 9: {
                display_all_teams(&db);
                break;
            }
            case 10: {
                display_all_games(&db);
                break;
            }
            case 11: {
                display_all_leagues(&db);
                break;
            }
            case 12: {
                char name[MAX_NAME_LENGTH];
                printf("Enter player name to search: ");
                scanf(" %[^\n]", name);
                search_players_by_name(&db, name);
                break;
            }
            case 13: {
                char city[50];
                printf("Enter team city to search: ");
                scanf(" %[^\n]", city);
                search_teams_by_city(&db, city);
                break;
            }
            case 14: {
                int team_id;
                printf("Enter team ID: ");
                scanf("%d", &team_id);
                search_games_by_team(&db, team_id);
                break;
            }
            case 15: {
                int player_id;
                printf("Enter player ID: ");
                scanf("%d", &player_id);
                generate_player_report(&db, player_id);
                break;
            }
            case 16: {
                int team_id;
                printf("Enter team ID: ");
                scanf("%d", &team_id);
                generate_team_report(&db, team_id);
                break;
            }
            case 17: {
                int league_id;
                printf("Enter league ID: ");
                scanf("%d", &league_id);
                generate_league_table(&db, league_id);
                break;
            }
            case 18: {
                int league_id;
                printf("Enter league ID: ");
                scanf("%d", &league_id);
                generate_top_scorers(&db, league_id);
                break;
            }
            case 19: {
                int league_id;
                printf("Enter league ID: ");
                scanf("%d", &league_id);
                generate_fixtures(&db, league_id);
                break;
            }
            case 20: {
                int league_id, num_games;
                printf("Enter league ID: ");
                scanf("%d", &league_id);
                printf("Enter number of games to simulate: ");
                scanf("%d", &num_games);
                simulate_games(&db, league_id, num_games);
                break;
            }
            case 21: {
                int player_id, goals, assists, yellow_cards, red_cards;
                
                printf("Enter player ID: ");
                scanf("%d", &player_id);
                printf("Enter goals: ");
                scanf("%d", &goals);
                printf("Enter assists: ");
                scanf("%d", &assists);
                printf("Enter yellow cards: ");
                scanf("%d", &yellow_cards);
                printf("Enter red cards: ");
                scanf("%d", &red_cards);
                
                update_player_stats(&db, player_id, goals, assists, yellow_cards, red_cards);
                break;
            }
            case 22: {
                if (save_database(&db)) {
                    printf("Database saved successfully.\n");
                } else {
                    printf("Failed to save database.\n");
                }
                break;
            }
            case 23: {
                if (load_database(&db)) {
                    printf("Database loaded successfully.\n");
                } else {
                    printf("Failed to load database.\n");
                }
                break;
            }
            case 24: {
                generate_sample_data(&db);
                printf("Sample data generated.\n");
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

void initialize_database(FootballDatabase *db) {
    <｜fim▁hole｜>db->player_count = 0;
    db->team_count = 0;
    db->game_count = 0;
    db->league_count = 0;
}

int add_player(FootballDatabase *db, const char *first_name, const char *last_name,
               const char *position, int age, double height, double weight,
               int team_id, int jersey_number, const char *nationality, double market_value,
               int contract_exp_day, int contract_exp_month, int contract_exp_year) {
    if (db->player_count >= MAX_PLAYERS) {
        return -1;
    }
    
    // Validate team exists
    if (find_team_by_id(db, team_id) == -1) {
        return -1;
    }
    
    Player *player = &db->players[db->player_count];
    player->id = db->player_count + 1;
    strncpy(player->first_name, first_name, MAX_NAME_LENGTH - 1);
    player->first_name[MAX_NAME_LENGTH - 1] = '\0';
    strncpy(player->last_name, last_name, MAX_NAME_LENGTH - 1);
    player->last_name[MAX_NAME_LENGTH - 1] = '\0';
    strncpy(player->position, position, 19);
    player->position[19] = '\0';
    player->age = age;
    player->height = height;
    player->weight = weight;
    player->team_id = team_id;
    player->jersey_number = jersey_number;
    player->active = 1;
    player->goals_scored = 0;
    player->assists = 0;
    player->yellow_cards = 0;
    player->red_cards = 0;
    player->games_played = 0;
    player->market_value = market_value;
    
    Date contract_expiration = {contract_exp_day, contract_exp_month, contract_exp_year};
    player->contract_expiration = contract_expiration;
    
    strncpy(player->nationality, nationality, 49);
    player->nationality[49] = '\0';
    
    db->player_count++;
    return player->id;
}

int add_team(FootballDatabase *db, const char *name, const char *city, const char *stadium,
             int stadium_capacity, const char *manager_name, int league_id, 
             const char *colors, int founded_year, double budget) {
    if (db->team_count >= MAX_TEAMS) {
        return -1;
    }
    
    // Validate league exists
    if (find_league_by_id(db, league_id) == -1) {
        return -1;
    }
    
    Team *team = &db->teams[db->team_count];
    team->id = db->team_count + 1;
    strncpy(team->name, name, MAX_NAME_LENGTH - 1);
    team->name[MAX_NAME_LENGTH - 1] = '\0';
    strncpy(team->city, city, 49);
    team->city[49] = '\0';
    strncpy(team->stadium, stadium, MAX_NAME_LENGTH - 1);
    team->stadium[MAX_NAME_LENGTH - 1] = '\0';
    team->stadium_capacity = stadium_capacity;
    strncpy(team->manager_name, manager_name, MAX_NAME_LENGTH - 1);
    team->manager_name[MAX_NAME_LENGTH - 1] = '\0';
    team->league_id = league_id;
    strncpy(team->colors, colors, 49);
    team->colors[49] = '\0';
    team->founded_year = founded_year;
    team->active = 1;
    team->points = 0;
    team->games_played = 0;
    team->wins = 0;
    team->draws = 0;
    team->losses = 0;
    team->goals_for = 0;
    team->goals_against = 0;
    team->budget = budget;
    
    db->team_count++;
    return team->id;
}

int add_league(FootballDatabase *db, const char *name, const char *country,
               const char *description, int teams_count, int games_per_team,
               int start_month, int end_month) {
    if (db->league_count >= MAX_LEAGUES) {
        return -1;
    }
    
    League *league = &db->leagues[db->league_count];
    league->id = db->league_count + 1;
    strncpy(league->name, name, MAX_NAME_LENGTH - 1);
    league->name[MAX_NAME_LENGTH - 1] = '\0';
    strncpy(league->country, country, 49);
    league->country[49] = '\0';
    strncpy(league->description, description, 199);
    league->description[199] = '\0';
    league->teams_count = teams_count;
    league->games_per_team = games_per_team;
    league->active = 1;
    league->start_month = start_month;
    league->end_month = end_month;
    
    db->league_count++;
    return league->id;
}

int add_game(FootballDatabase *db, int home_team_id, int away_team_id, 
             int day, int month, int year, const char *time, int league_id, 
             int week_number, const char *venue) {
    if (db->game_count >= MAX_GAMES) {
        return -1;
    }
    
    // Validate teams and league exist
    if (find_team_by_id(db, home_team_id) == -1 || 
        find_team_by_id(db, away_team_id) == -1 || 
        find_league_by_id(db, league_id) == -1) {
        return -1;
    }
    
    Game *game = &db->games[db->game_count];
    game->id = db->game_count + 1;
    game->home_team_id = home_team_id;
    game->away_team_id = away_team_id;
    
    Date date = {day, month, year};
    game->date = date;
    
    strncpy(game->time, time, 9);
    game->time[9] = '\0';
    game->league_id = league_id;
    game->week_number = week_number;
    game->finished = 0;
    game->home_score = 0;
    game->away_score = 0;
    strncpy(game->venue, venue, MAX_NAME_LENGTH - 1);
    game->venue[MAX_NAME_LENGTH - 1] = '\0';
    game->attendance = 0;
    
    db->game_count++;
    return game->id;
}

void update_game_score(FootballDatabase *db, int game_id, int home_score, int away_score) {
    int game_index = find_game_by_id(db, game_id);
    if (game_index == -1) {
        printf("Game not found.\n");
        return;
    }
    
    Game *game = &db->games[game_index];
    game->home_score = home_score;
    game->away_score = away_score;
    game->finished = 1;
    
    // Update team statistics
    int home_team_index = find_team_by_id(db, game->home_team_id);
    int away_team_index = find_team_by_id(db, game->away_team_id);
    
    if (home_team_index != -1 && away_team_index != -1) {
        Team *home_team = &db->teams[home_team_index];
        Team *away_team = &db->teams[away_team_index];
        
        // Update games played
        home_team->games_played++;
        away_team->games_played++;
        
        // Update goals
        home_team->goals_for += home_score;
        home_team->goals_against += away_score;
        away_team->goals_for += away_score;
        away_team->goals_against += home_score;
        
        // Update wins/draws/losses and points
        if (home_score > away_score) {
            home_team->wins++;
            home_team->points += 3;
            away_team->losses++;
        } else if (home_score < away_score) {
            away_team->wins++;
            away_team->points += 3;
            home_team->losses++;
        } else {
            home_team->draws++;
            away_team->draws++;
            home_team->points += 1;
            away_team->points += 1;
        }
    }
    
    printf("Game #%d score updated: %d-%d\n", game_id, home_score, away_score);
}

void transfer_player(FootballDatabase *db, int player_id, int new_team_id, double transfer_fee) {
    int player_index = find_player_by_id(db, player_id);
    int new_team_index = find_team_by_id(db, new_team_id);
    
    if (player_index == -1) {
        printf("Player not found.\n");
        return;
    }
    
    if (new_team_index == -1) {
        printf("New team not found.\n");
        return;
    }
    
    Player *player = &db->players[player_index];
    int old_team_id = player->team_id;
    player->team_id = new_team_id;
    
    // Generate a new jersey number (random for simplicity)
    player->jersey_number = 1 + (rand() % 99);
    
    // Update team budgets
    int old_team_index = find_team_by_id(db, old_team_id);
    if (old_team_index != -1) {
        db->teams[old_team_index].budget += transfer_fee;
    }
    
    db->teams[new_team_index].budget -= transfer_fee;
    
    printf("Player transferred successfully.\n");
    printf("Player: %s %s\n", player->first_name, player->last_name);
    printf("From Team ID: %d\n", old_team_id);
    printf("To Team ID: %d\n", new_team_id);
    printf("Transfer Fee: %.2f million\n", transfer_fee);
    printf("New Jersey Number: %d\n", player->jersey_number);
}

void update_team_stats(FootballDatabase *db) {
    printf("Updating team statistics...\n");
    
    // Reset team statistics
    for (int i = 0; i < db->team_count; i++) {
        db->teams[i].points = 0;
        db->teams[i].games_played = 0;
        db->teams[i].wins = 0;
        db->teams[i].draws = 0;
        db->teams[i].losses = 0;
        db->teams[i].goals_for = 0;
        db->teams[i].goals_against = 0;
    }
    
    // Recalculate statistics from games
    for (int i = 0; i < db->game_count; i++) {
        if (db->games[i].finished) {
            int home_team_index = find_team_by_id(db, db->games[i].home_team_id);
            int away_team_index = find_team_by_id(db, db->games[i].away_team_id);
            
            if (home_team_index != -1 && away_team_index != -1) {
                Team *home_team = &db->teams[home_team_index];
                Team *away_team = &db->teams[away_team_index];
                
                // Update games played
                home_team->games_played++;
                away_team->games_played++;
                
                // Update goals
                home_team->goals_for += db->games[i].home_score;
                home_team->goals_against += db->games[i].away_score;
                away_team->goals_for += db->games[i].away_score;
                away_team->goals_against += db->games[i].home_score;
                
                // Update wins/draws/losses and points
                if (db->games[i].home_score > db->games[i].away_score) {
                    home_team->wins++;
                    home_team->points += 3;
                    away_team->losses++;
                } else if (db->games[i].home_score < db->games[i].away_score) {
                    away_team->wins++;
                    away_team->points += 3;
                    home_team->losses++;
                } else {
                    home_team->draws++;
                    away_team->draws++;
                    home_team->points += 1;
                    away_team->points += 1;
                }
            }
        }
    }
    
    printf("Team statistics updated.\n");
}

void display_player(const Player *player, const FootballDatabase *db) {
    printf("\nPlayer ID: %d\n", player->id);
    printf("Name: %s %s\n", player->first_name, player->last_name);
    printf("Position: %s\n", player->position);
    printf("Description: %s\n", get_position_description(player->position));
    printf("Age: %d\n", player->age);
    printf("Height: %.2f m\n", player->height);
    printf("Weight: %.1f kg\n", player->weight);
    printf("Nationality: %s\n", player->nationality);
    printf("Jersey Number: %d\n", player->jersey_number);
    printf("Market Value: %.2f million\n", player->market_value);
    printf("Contract Expires: %02d/%02d/%d\n", 
           player->contract_expiration.day, player->contract_expiration.month, player->contract_expiration.year);
    
    // Display team name
    int team_index = find_team_by_id(db, player->team_id);
    if (team_index != -1) {
        printf("Team: %s\n", db->teams[team_index].name);
    }
    
    printf("Games Played: %d\n", player->games_played);
    printf("Goals Scored: %d\n", player->goals_scored);
    printf("Assists: %d\n", player->assists);
    printf("Yellow Cards: %d\n", player->yellow_cards);
    printf("Red Cards: %d\n", player->red_cards);
    printf("Status: %s\n", player->active ? "Active" : "Inactive");
}

void display_team(const Team *team, const FootballDatabase *db) {
    printf("\nTeam ID: %d\n", team->id);
    printf("Name: %s\n", team->name);
    printf("City: %s\n", team->city);
    printf("Stadium: %s\n", team->stadium);
    printf("Stadium Capacity: %d\n", team->stadium_capacity);
    printf("Manager: %s\n", team->manager_name);
    printf("Colors: %s\n", team->colors);
    printf("Founded: %d\n", team->founded_year);
    
    // Display league name
    int league_index = find_league_by_id(db, team->league_id);
    if (league_index != -1) {
        printf("League: %s\n", db->leagues[league_index].name);
    }
    
    printf("Budget: %.2f million\n", team->budget);
    printf("Games Played: %d\n", team->games_played);
    printf("Wins: %d\n", team->wins);
    printf("Draws: %d\n", team->draws);
    printf("Losses: %d\n", team->losses);
    printf("Points: %d\n", team->points);
    printf("Goals For: %d\n", team->goals_for);
    printf("Goals Against: %d\n", team->goals_against);
    printf("Goal Difference: %d\n", team->goals_for - team->goals_against);
    printf("Status: %s\n", team->active ? "Active" : "Inactive");
}

void display_game(const Game *game, const FootballDatabase *db) {
    printf("\nGame ID: %d\n", game->id);
    
    // Display team names
    int home_team_index = find_team_by_id(db, game->home_team_id);
    int away_team_index = find_team_by_id(db, game->away_team_id);
    
    if (home_team_index != -1) {
        printf("Home Team: %s\n", db->teams[home_team_index].name);
    }
    
    if (away_team_index != -1) {
        printf("Away Team: %s\n", db->teams[away_team_index].name);
    }
    
    printf("Date: %02d/%02d/%d\n", game->date.day, game->date.month, game->date.year);
    printf("Time: %s\n", game->time);
    printf("Venue: %s\n", game->venue);
    printf("Week: %d\n", game->week_number);
    
    // Display league name
    int league_index = find_league_by_id(db, game->league_id);
    if (league_index != -1) {
        printf("League: %s\n", db->leagues[league_index].name);
    }
    
    if (game->finished) {
        printf("Score: %d - %d\n", game->home_score, game->away_score);
        printf("Attendance: %d\n", game->attendance);
    } else {
        printf("Status: Not played yet\n");
    }
}

void display_league(const League *league) {
    printf("\nLeague ID: %d\n", league->id);
    printf("Name: %s\n", league->name);
    printf("Country: %s\n", league->country);
    printf("Description: %s\n", league->description);
    printf("Teams Count: %d\n", league->teams_count);
    printf("Games per Team: %d\n", league->games_per_team);
    printf("Season: %d - %d\n", league->start_month, league->end_month);
    printf("Status: %s\n", league->active ? "Active" : "Inactive");
}

void display_all_players(const FootballDatabase *db) {
    printf("\n=== All Players ===\n");
    printf("Total Players: %d\n\n", db->player_count);
    
    for (int i = 0; i < db->player_count; i++) {
        if (db->players[i].active) {
            display_player(&db->players[i], db);
            printf("------------------------\n");
        }
    }
}

void display_all_teams(const FootballDatabase *db) {
    printf("\n=== All Teams ===\n");
    printf("Total Teams: %d\n\n", db->team_count);
    
    for (int i = 0; i < db->team_count; i++) {
        if (db->teams[i].active) {
            display_team(&db->teams[i], db);
            printf("------------------------\n");
        }
    }
}

void display_all_games(const FootballDatabase *db) {
    printf("\n=== All Games ===\n");
    printf("Total Games: %d\n\n", db->game_count);
    
    for (int i = 0; i < db->game_count; i++) {
        display_game(&db->games[i], db);
        printf("------------------------\n");
    }
}

void display_all_leagues(const FootballDatabase *db) {
    printf("\n=== All Leagues ===\n");
    printf("Total Leagues: %d\n\n", db->league_count);
    
    for (int i = 0; i < db->league_count; i++) {
        if (db->leagues[i].active) {
            display_league(&db->leagues[i]);
            printf("------------------------\n");
        }
    }
}

void search_players_by_name(const FootballDatabase *db, const char *name) {
    printf("\nSearch Results for '%s':\n", name);
    int found = 0;
    
    for (int i = 0; i < db->player_count; i++) {
        if (db->players[i].active && 
            (strstr(db->players[i].first_name, name) != NULL || 
             strstr(db->players[i].last_name, name) != NULL)) {
            display_player(&db->players[i], db);
            printf("------------------------\n");
            found++;
        }
    }
    
    if (found == 0) {
        printf("No players found with name containing '%s'.\n", name);
    }
}

void search_teams_by_city(const FootballDatabase *db, const char *city) {
    printf("\nTeams in City: %s\n", city);
    int found = 0;
    
    for (int i = 0; i < db->team_count; i++) {
        if (db->teams[i].active && strstr(db->teams[i].city, city) != NULL) {
            display_team(&db->teams[i], db);
            printf("------------------------\n");
            found++;
        }
    }
    
    if (found == 0) {
        printf("No teams found in this city.\n");
    }
}

void search_games_by_team(const FootballDatabase *db, int team_id) {
    int team_index = find_team_by_id(db, team_id);
    if (team_index == -1) {
        printf("Team not found.\n");
        return;
    }
    
    printf("\nGames for Team: %s (ID: %d)\n", 
           db->teams[team_index].name, team_id);
    
    int found = 0;
    
    for (int i = 0; i < db->game_count; i++) {
        if (db->games[i].home_team_id == team_id || db->games[i].away_team_id == team_id) {
            display_game(&db->games[i], db);
            printf("------------------------\n");
            found++;
        }
    }
    
    if (found == 0) {
        printf("No games found for this team.\n");
    }
}

void generate_player_report(const FootballDatabase *db, int player_id) {
    int player_index = find_player_by_id(db, player_id);
    if (player_index == -1) {
        printf("Player not found.\n");
        return;
    }
    
    Player *player = &db->players[player_index];
    
    printf("\n=== Player Report: %s %s ===\n", player->first_name, player->last_name);
    printf("Player ID: %d\n", player->id);
    printf("Position: %s\n", player->position);
    printf("Description: %s\n", get_position_description(player->position));
    printf("Age: %d\n", player->age);
    printf("Height: %.2f m\n", player->height);
    printf("Weight: %.1f kg\n", player->weight);
    printf("Nationality: %s\n", player->nationality);
    printf("Jersey Number: %d\n", player->jersey_number);
    printf("Market Value: %.2f million\n", player->market_value);
    printf("Contract Expires: %02d/%02d/%d\n", 
           player->contract_expiration.day, player->contract_expiration.month, player->contract_expiration.year);
    
    // Display team name
    int team_index = find_team_by_id(db, player->team_id);
    if (team_index != -1) {
        printf("Team: %s\n", db->teams[team_index].name);
    }
    
    printf("\nCareer Statistics:\n");
    printf("Games Played: %d\n", player->goals_scored);
    printf("Goals Scored: %d\n", player->goals_scored);
    printf("Assists: %d\n", player->assists);
    printf("Yellow Cards: %d\n", player->yellow_cards);
    printf("Red Cards: %d\n", player->red_cards);
    
    // Calculate performance metrics
    if (player->games_played > 0) {
        printf("\nPerformance Metrics:\n");
        printf("Goals per Game: %.2f\n", (float)player->goals_scored / player->games_played);
        printf("Assists per Game: %.2f\n", (float)player->assists / player->games_played);
        printf("Goal Contributions per Game: %.2f\n", 
               (float)(player->goals_scored + player->assists) / player->games_played);
        printf("Cards per Game: %.2f\n", 
               (float)(player->yellow_cards + player->red_cards) / player->games_played);
    }
    
    // Position-specific statistics
    if (strcmp(player->position, "Goalkeeper") == 0 && player->games_played > 0) {
        printf("\nGoalkeeper Specific:\n");
        printf("Clean Sheets: %d\n", player->goals_scored); // Using goals_scored as clean sheets for GK
        printf("Clean Sheet Percentage: %.1f%%\n", 
               (float)player->goals_scored / player->games_played * 100);
    }
    
    // Recent games
    printf("\nRecent Games:\n");
    printf("%-15s %-10s %-10s %-10s\n", "Date", "Opponent", "Result", "Performance");
    printf("------------------------------------------------\n");
    
    int games_found = 0;
    for (int i = 0; i < db->game_count && games_found < 5; i++) {
        if (db->games[i].finished) {
            int opponent_team_id = -1;
            int is_home = 0;
            int result = 0; // 0: draw, 1: win, -1: loss
            
            if (db->games[i].home_team_id == player->team_id) {
                opponent_team_id = db->games[i].away_team_id;
                is_home = 1;
                if (db->games[i].home_score > db->games[i].away_score) {
                    result = 1;
                } else if (db->games[i].home_score < db->games[i].away_score) {
                    result = -1;
                }
            } else if (db->games[i].away_team_id == player->team_id) {
                opponent_team_id = db->games[i].home_team_id;
                is_home = 0;
                if (db->games[i].away_score > db->games[i].home_score) {
                    result = 1;
                } else if (db->games[i].away_score < db->games[i].home_score) {
                    result = -1;
                }
            }
            
            if (opponent_team_id != -1) {
                int opponent_index = find_team_by_id(db, opponent_team_id);
                if (opponent_index != -1) {
                    char result_str[10];
                    if (result == 1) {
                        strcpy(result_str, "Win");
                    } else if (result == -1) {
                        strcpy(result_str, "Loss");
                    } else {
                        strcpy(result_str, "Draw");
                    }
                    
                    printf("%02d/%02d/%d %-10s %-10s %-10s\n", 
                           db->games[i].date.day, db->games[i].date.month, db->games[i].date.year,
                           db->teams[opponent_index].name, result_str, "Good");
                    
                    games_found++;
                }
            }
        }
    }
    
    if (games_found == 0) {
        printf("No recent games found.\n");
    }
}

void generate_team_report(const FootballDatabase *db, int team_id) {
    int team_index = find_team_by_id(db, team_id);
    if (team_index == -1) {
        printf("Team not found.\n");
        return;
    }
    
    Team *team = &db->teams[team_index];
    
    printf("\n=== Team Report: %s ===\n", team->name);
    printf("Team ID: %d\n", team->id);
    printf("City: %s\n", team->city);
    printf("Stadium: %s\n", team->stadium);
    printf("Stadium Capacity: %d\n", team->stadium_capacity);
    printf("Manager: %s\n", team->manager_name);
    printf("Colors: %s\n", team->colors);
    printf("Founded: %d\n", team->founded_year);
    
    // Display league name
    int league_index = find_league_by_id(db, team->league_id);
    if (league_index != -1) {
        printf("League: %s\n", db->leagues[league_index].name);
    }
    
    printf("\nFinancial Information:\n");
    printf("Budget: %.2f million\n", team->budget);
    
    printf("\nSeason Performance:\n");
    printf("Games Played: %d\n", team->games_played);
    printf("Wins: %d\n", team->wins);
    printf("Draws: %d\n", team->draws);
    printf("Losses: %d\n", team->losses);
    printf("Points: %d\n", team->points);
    printf("Goals For: %d\n", team->goals_for);
    printf("Goals Against: %d\n", team->goals_against);
    printf("Goal Difference: %d\n", team->goals_for - team->goals_against);
    
    // Calculate performance metrics
    if (team->games_played > 0) {
        printf("\nPerformance Metrics:\n");
        printf("Win Percentage: %.1f%%\n", (float)team->wins / team->games_played * 100);
        printf("Draw Percentage: %.1f%%\n", (float)team->draws / team->games_played * 100);
        printf("Loss Percentage: %.1f%%\n", (float)team->losses / team->games_played * 100);
        printf("Points per Game: %.2f\n", (float)team->points / team->games_played);
        printf("Goals per Game: %.2f\n", (float)team->goals_for / team->games_played);
        printf("Goals Conceded per Game: %.2f\n", (float)team->goals_against / team->games_played);
    }
    
    // Player roster
    printf("\nPlayer Roster:\n");
    printf("%-5s %-20s %-15s %-10s %-10s %-10s %-10s\n", 
           "ID", "Name", "Position", "Age", "Jersey", "Goals", "Assists");
    printf("--------------------------------------------------------------------------------\n");
    
    int players_found = 0;
    for (int i = 0; i < db->player_count; i++) {
        if (db->players[i].active && db->players[i].team_id == team_id) {
            printf("%-5d %-20s %-15s %-10d %-10d %-10d %-10d\n", 
                   db->players[i].id,
                   db->players[i].first_name,
                   db->players[i].position,
                   db->players[i].age,
                   db->players[i].jersey_number,
                   db->players[i].goals_scored,
                   db->players[i].assists);
            
            players_found++;
        }
    }
    
    if (players_found == 0) {
        printf("No players found on this team.\n");
    }
    
    // Recent games
    printf("\nRecent Games:\n");
    printf("%-15s %-15s %-10s %-10s\n", "Date", "Opponent", "Result", "Score");
    printf("------------------------------------------------\n");
    
    int games_found = 0;
    for (int i = 0; i < db->game_count && games_found < 5; i++) {
        if (db->games[i].finished) {
            int opponent_team_id = -1;
            int is_home = 0;
            int result = 0; // 0: draw, 1: win, -1: loss
            
            if (db->games[i].home_team_id == team_id) {
                opponent_team_id = db->games[i].away_team_id;
                is_home = 1;
                if (db->games[i].home_score > db->games[i].away_score) {
                    result = 1;
                } else if (db->games[i].home_score < db->games[i].away_score) {
                    result = -1;
                }
            } else if (db->games[i].away_team_id == team_id) {
                opponent_team_id = db->games[i].home_team_id;
                is_home = 0;
                if (db->games[i].away_score > db->games[i].home_score) {
                    result = 1;
                } else if (db->games[i].away_score < db->games[i].home_score) {
                    result = -1;
                }
            }
            
            if (opponent_team_id != -1) {
                int opponent_index = find_team_by_id(db, opponent_team_id);
                if (opponent_index != -1) {
                    char result_str[10];
                    if (result == 1) {
                        strcpy(result_str, "Win");
                    } else if (result == -1) {
                        strcpy(result_str, "Loss");
                    } else {
                        strcpy(result_str, "Draw");
                    }
                    
                    if (is_home) {
                        printf("%02d/%02d/%d %-15s %-10s %d-%d\n", 
                               db->games[i].date.day, db->games[i].date.month, db->games[i].date.year,
                               db->teams[opponent_index].name, result_str,
                               db->games[i].home_score, db->games[i].away_score);
                    } else {
                        printf("%02d/%02d/%d %-15s %-10s %d-%d\n", 
                               db->games[i].date.day, db->games[i].date.month, db->games[i].date.year,
                               db->teams[opponent_index].name, result_str,
                               db->games[i].away_score, db->games[i].home_score);
                    }
                    
                    games_found++;
                }
            }
        }
    }
    
    if (games_found == 0) {
        printf("No recent games found.\n");
    }
}

void generate_league_table(const FootballDatabase *db, int league_id) {
    int league_index = find_league_by_id(db, league_id);
    if (league_index == -1) {
        printf("League not found.\n");
        return;
    }
    
    League *league = &db->leagues[league_index];
    
    printf("\n=== League Table: %s ===\n", league->name);
    printf("Country: %s\n", league->country);
    printf("Description: %s\n", league->description);
    
    // Create array to hold team indices and sort by points
    typedef struct {
        int team_index;
        int points;
        int goal_difference;
        int goals_for;
    } TeamTableEntry;
    
    TeamTableEntry table[MAX_TEAMS];
    int table_size = 0;
    
    // Populate table with teams in this league
    for (int i = 0; i < db->team_count; i++) {
        if (db->teams[i].active && db->teams[i].league_id == league_id) {
            table[table_size].team_index = i;
            table[table_size].points = db->teams[i].points;
            table[table_size].goal_difference = db->teams[i].goals_for - db->teams[i].goals_against;
            table[table_size].goals_for = db->teams[i].goals_for;
            table_size++;
        }
    }
    
    // Sort table (by points, then goal difference, then goals for)
    for (int i = 0; i < table_size - 1; i++) {
        for (int j = 0; j < table_size - i - 1; j++) {
            if (table[j].points < table[j + 1].points ||
                (table[j].points == table[j + 1].points && 
                 table[j].goal_difference < table[j + 1].goal_difference) ||
                (table[j].points == table[j + 1].points && 
                 table[j].goal_difference == table[j + 1].goal_difference && 
                 table[j].goals_for < table[j + 1].goals_for)) {
                TeamTableEntry temp = table[j];
                table[j] = table[j + 1];
                table[j + 1] = temp;
            }
        }
    }
    
    // Display table
    printf("\n%-5s %-20s %-10s %-10s %-10s %-10s %-10s %-10s %-10s\n", 
           "Pos", "Team", "P", "W", "D", "L", "GF", "GA", "GD", "Pts");
    printf("--------------------------------------------------------------------------------\n");
    
    for (int i = 0; i < table_size; i++) {
        int team_index = table[i].team_index;
        printf("%-5d %-20s %-10d %-10d %-10d %-10d %-10d %-10d %-10d %-10d\n", 
               i + 1,
               db->teams[team_index].name,
               db->teams[team_index].games_played,
               db->teams[team_index].wins,
               db->teams[team_index].draws,
               db->teams[team_index].losses,
               db->teams[team_index].goals_for,
               db->teams[team_index].goals_against,
               db->teams[team_index].goals_for - db->teams[team_index].goals_against,
               db->teams[team_index].points);
    }
    
    if (table_size == 0) {
        printf("No teams found in this league.\n");
    }
}

void generate_top_scorers(const FootballDatabase *db, int league_id) {
    int league_index = find_league_by_id(db, league_id);
    if (league_index == -1) {
        printf("League not found.\n");
        return;
    }
    
    printf("\n=== Top Scorers: %s ===\n", db->leagues[league_index].name);
    
    // Create array to hold player indices and sort by goals
    typedef struct {
        int player_index;
        int goals_scored;
        int assists;
        int games_played;
    } PlayerScorer;
    
    PlayerScorer scorers[MAX_PLAYERS];
    int scorer_count = 0;
    
    // Populate array with players in this league
    for (int i = 0; i < db->player_count; i++) {
        if (db->players[i].active) {
            int team_index = find_team_by_id(db, db->players[i].team_id);
            if (team_index != -1 && db->teams[team_index].league_id == league_id) {
                scorers[scorer_count].player_index = i;
                scorers[scorer_count].goals_scored = db->players[i].goals_scored;
                scorers[scorer_count].assists = db->players[i].assists;
                scorers[scorer_count].games_played = db->players[i].games_played;
                scorer_count++;
            }
        }
    }
    
    // Sort scorers (by goals, then assists)
    for (int i = 0; i < scorer_count - 1; i++) {
        for (int j = 0; j < scorer_count - i - 1; j++) {
            if (scorers[j].goals_scored < scorers[j + 1].goals_scored ||
                (scorers[j].goals_scored == scorers[j + 1].goals_scored && 
                 scorers[j].assists < scorers[j + 1].assists)) {
                PlayerScorer temp = scorers[j];
                scorers[j] = scorers[j + 1];
                scorers[j + 1] = temp;
            }
        }
    }
    
    // Display top scorers
    printf("\n%-5s %-20s %-15s %-10s %-10s %-10s %-15s\n", 
           "Pos", "Name", "Team", "Goals", "Assists", "Games", "Goals/Game");
    printf("--------------------------------------------------------------------------------\n");
    
    for (int i = 0; i < 20 && i < scorer_count; i++) { // Top 20 scorers
        int player_index = scorers[i].player_index;
        int team_index = find_team_by_id(db, db->players[player_index].team_id);
        
        float goals_per_game = 0.0f;
        if (scorers[i].games_played > 0) {
            goals_per_game = (float)scorers[i].goals_scored / scorers[i].games_played;
        }
        
        printf("%-5d %-20s %-15s %-10d %-10d %-10d %-15.2f\n", 
               i + 1,
               db->players[player_index].first_name,
               team_index != -1 ? db->teams[team_index].name : "Unknown",
               scorers[i].goals_scored,
               scorers[i].assists,
               scorers[i].games_played,
               goals_per_game);
    }
    
    if (scorer_count == 0) {
        printf("No players found in this league.\n");
    }
}

void generate_fixtures(const FootballDatabase *db, int league_id) {
    int league_index = find_league_by_id(db, league_id);
    if (league_index == -1) {
        printf("League not found.\n");
        return;
    }
    
    printf("\n=== Fixtures: %s ===\n", db->leagues[league_index].name);
    
    // Get all games for this league
    typedef struct {
        int game_index;
        int week_number;
        int date_timestamp; // For sorting
    } GameEntry;
    
    GameEntry games[MAX_GAMES];
    int game_count = 0;
    
    for (int i = 0; i < db->game_count; i++) {
        if (db->games[i].league_id == league_id) {
            games[game_count].game_index = i;
            games[game_count].week_number = db->games[i].week_number;
            games[game_count].date_timestamp = db->games[i].date.year * 10000 + 
                                           db->games[i].date.month * 100 + 
                                           db->games[i].date.day;
            game_count++;
        }
    }
    
    // Sort games by week, then date
    for (int i = 0; i < game_count - 1; i++) {
        for (int j = 0; j < game_count - i - 1; j++) {
            if (games[j].week_number > games[j + 1].week_number ||
                (games[j].week_number == games[j + 1].week_number && 
                 games[j].date_timestamp > games[j + 1].date_timestamp)) {
                GameEntry temp = games[j];
                games[j] = games[j + 1];
                games[j + 1] = temp;
            }
        }
    }
    
    // Display fixtures by week
    int current_week = -1;
    
    for (int i = 0; i < game_count; i++) {
        int game_index = games[i].game_index;
        Game *game = &db->games[game_index];
        
        if (game->week_number != current_week) {
            current_week = game->week_number;
            printf("\nWeek %d:\n", current_week);
            printf("------------------------------------------------\n");
        }
        
        // Get team names
        int home_team_index = find_team_by_id(db, game->home_team_id);
        int away_team_index = find_team_by_id(db, game->away_team_id);
        
        if (home_team_index != -1 && away_team_index != -1) {
            printf("%02d/%02d/%d %s: ", 
                   game->date.day, game->date.month, game->date.year, game->time);
            
            printf("%-20s vs %-20s ", 
                   db->teams[home_team_index].name, 
                   db->teams[away_team_index].name);
            
            if (game->finished) {
                printf("(%d-%d)", game->home_score, game->away_score);
            } else {
                printf("(Not played)");
            }
            
            printf("\n");
        }
    }
    
    if (game_count == 0) {
        printf("No fixtures found for this league.\n");
    }
}

void simulate_games(FootballDatabase *db, int league_id, int num_games) {
    int league_index = find_league_by_id(db, league_id);
    if (league_index == -1) {
        printf("League not found.\n");
        return;
    }
    
    printf("\nSimulating games for league: %s\n", db->leagues[league_index].name);
    
    // Find games that haven't been played yet
    int unplayed_games[MAX_GAMES];
    int unplayed_count = 0;
    
    for (int i = 0; i < db->game_count; i++) {
        if (db->games[i].league_id == league_id && !db->games[i].finished) {
            unplayed_games[unplayed_count++] = i;
        }
    }
    
    if (unplayed_count == 0) {
        printf("No unplayed games found in this league.\n");
        return;
    }
    
    // Simulate the requested number of games (or all if fewer available)
    int games_to_simulate = num_games > unplayed_count ? unplayed_count : num_games;
    
    for (int i = 0; i < games_to_simulate; i++) {
        int game_index = unplayed_games[rand() % unplayed_count];
        Game *game = &db->games[game_index];
        
        // Simulate score (simplified model)
        int home_score = rand() % 5;
        int away_score = rand() % 5;
        
        // Add some randomness for more interesting scores
        if (rand() % 10 < 3) { // 30% chance of extra goals
            if (rand() % 2) {
                home_score += rand() % 3;
            } else {
                away_score += rand() % 3;
            }
        }
        
        printf("Simulating: %s vs %s\n", 
               db->teams[find_team_by_id(db, game->home_team_id)].name,
               db->teams[find_team_by_id(db, game->away_team_id)].name);
        printf("Result: %d-%d\n", home_score, away_score);
        
        // Update game score
        update_game_score(db, game->id, home_score, away_score);
        
        // Simulate player stats (simplified)
        for (int j = 0; j < db->player_count; j++) {
            if (db->players[j].team_id == game->home_team_id || db->players[j].team_id == game->away_team_id) {
                if (rand() % 3 == 0) { // 33% chance of scoring
                    db->players[j].goals_scored += rand() % 3;
                }
                if (rand() % 4 == 0) { // 25% chance of assisting
                    db->players[j].assists += rand() % 2;
                }
                if (rand() % 10 < 2) { // 20% chance of yellow card
                    db->players[j].yellow_cards++;
                }
                if (rand() % 100 < 2) { // 2% chance of red card
                    db->players[j].red_cards++;
                }
                db->players[j].games_played++;
            }
        }
        
        // Set attendance (random between 50% and 100% of stadium capacity)
        int home_team_index = find_team_by_id(db, game->home_team_id);
        if (home_team_index != -1) {
            int capacity = db->teams[home_team_index].stadium_capacity;
            game->attendance = capacity * (50 + rand() % 51) / 100;
        }
    }
    
    printf("\nSimulation complete. %d games simulated.\n", games_to_simulate);
}

void update_player_stats(FootballDatabase *db, int player_id, int goals, int assists, 
                         int yellow_cards, int red_cards) {
    int player_index = find_player_by_id(db, player_id);
    if (player_index == -1) {
        printf("Player not found.\n");
        return;
    }
    
    Player *player = &db->players[player_index];
    
    player->goals_scored += goals;
    player->assists += assists;
    player->yellow_cards += yellow_cards;
    player->red_cards += red_cards;
    
    printf("Player statistics updated.\n");
    printf("Player: %s %s\n", player->first_name, player->last_name);
    printf("Goals Added: %d (Total: %d)\n", goals, player->goals_scored);
    printf("Assists Added: %d (Total: %d)\n", assists, player->assists);
    printf("Yellow Cards Added: %d (Total: %d)\n", yellow_cards, player->yellow_cards);
    printf("Red Cards Added: %d (Total: %d)\n", red_cards, player->red_cards);
}

int find_player_by_id(const FootballDatabase *db, int player_id) {
    for (int i = 0; i < db->player_count; i++) {
        if (db->players[i].id == player_id && db->players[i].active) {
            return i;
        }
    }
    return -1;
}

int find_team_by_id(const FootballDatabase *db, int team_id) {
    for (int i = 0; i < db->team_count; i++) {
        if (db->teams[i].id == team_id && db->teams[i].active) {
            return i;
        }
    }
    return -1;
}

int find_game_by_id(const FootballDatabase *db, int game_id) {
    for (int i = 0; i < db->game_count; i++) {
        if (db->games[i].id == game_id) {
            return i;
        }
    }
    return -1;
}

int find_league_by_id(const FootballDatabase *db, int league_id) {
    for (int i = 0; i < db->league_count; i++) {
        if (db->leagues[i].id == league_id && db->leagues[i].active) {
            return i;
        }
    }
    return -1;
}

int save_database(const FootballDatabase *db) {
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
    
    FILE *games_file = fopen(FILENAME_GAMES, "wb");
    if (games_file != NULL) {
        fwrite(&db->game_count, sizeof(int), 1, games_file);
        fwrite(db->games, sizeof(Game), db->game_count, games_file);
        fclose(games_file);
    } else {
        return 0;
    }
    
    FILE *leagues_file = fopen(FILENAME_LEAGUES, "wb");
    if (leagues_file != NULL) {
        fwrite(&db->league_count, sizeof(int), 1, leagues_file);
        fwrite(db->leagues, sizeof(League), db->league_count, leagues_file);
        fclose(leagues_file);
        return 1;
    }
    
    return 0;
}

int load_database(FootballDatabase *db) {
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
    
    FILE *games_file = fopen(FILENAME_GAMES, "rb");
    if (games_file != NULL) {
        fread(&db->game_count, sizeof(int), 1, games_file);
        fread(db->games, sizeof(Game), db->game_count, games_file);
        fclose(games_file);
    } else {
        return 0;
    }
    
    FILE *leagues_file = fopen(FILENAME_LEAGUES, "rb");
    if (leagues_file != NULL) {
        fread(&db->league_count, sizeof(int), 1, leagues_file);
        fread(db->leagues, sizeof(League), db->league_count, leagues_file);
        fclose(leagues_file);
        return 1;
    }
    
    return 0;
}

void generate_sample_data(FootballDatabase *db) {
    char* first_names[] = {"John", "Jane", "Michael", "Sarah", "Robert", "Emily",
                          "David", "Jessica", "William", "Ashley", "James", "Amanda",
                          "Daniel", "Jennifer", "Matthew", "Melissa", "Christopher", "Laura",
                          "Joseph", "Karen", "Thomas", "Nancy", "Charles", "Lisa"};
    
    char* last_names[] = {"Smith", "Johnson", "Williams", "Brown", "Jones", "Garcia",
                          "Miller", "Davis", "Rodriguez", "Martinez", "Wilson", "Anderson",
                          "Taylor", "Thomas", "Moore", "Jackson", "Martin", "Lee"};
    
    char* team_names[] = {"United", "City", "FC", "Athletic", "Wanderers", "Rovers",
                         "Hotspur", "Arsenal", "Chelsea", "Liverpool", "Barcelona", "Real Madrid"};
    
    char* cities[] = {"Manchester", "London", "Liverpool", "Birmingham", "Leeds", "Newcastle",
                     "Sheffield", "Bristol", "Nottingham", "Leicester", "Southampton", "Portsmouth"};
    
    char* stadiums[] = {"Old Trafford", "Etihad Stadium", "Anfield", "Stamford Bridge", "Emirates Stadium",
                       "Tottenham Hotspur Stadium", "St James' Park", "Villa Park", "Elland Road", "Goodison Park"};
    
    char* managers[] = {"Sir Alex Ferguson", "Pep Guardiola", "Jurgen Klopp", "Mauricio Pochettino",
                        "Jose Mourinho", "Antonio Conte", "Zinedine Zidane", "Diego Simeone"};
    
    char* nationalities[] = {"England", "Spain", "France", "Germany", "Italy", "Brazil", "Argentina",
                             "Netherlands", "Portugal", "Belgium"};
    
    srand(time(NULL));
    
    // Add leagues
    add_league(db, "Premier League", "England", "Top tier of English football", 20, 38, 8, 5);
    add_league(db, "La Liga", "Spain", "Top tier of Spanish football", 20, 38, 8, 5);
    add_league(db, "Serie A", "Italy", "Top tier of Italian football", 20, 38, 8, 5);
    
    // Add teams
    for (int i = 0; i < 30 && db->team_count < MAX_TEAMS; i++) {
        int league_id = 1 + (rand() % 3);
        char full_team_name[100];
        sprintf(full_team_name, "%s %s", cities[rand() % 12], team_names[rand() % 12]);
        
        add_team(db, full_team_name, cities[rand() % 12], stadiums[rand() % 10],
                 30000 + (rand() % 50000), managers[rand() % 8], league_id,
                 "Red and White", 1880 + (rand() % 140), 100.0 + (rand() % 400));
    }
    
    // Add players
    const char* positions[] = {"Forward", "Midfielder", "Defender", "Goalkeeper"};
    
    for (int i = 0; i < 500 && db->player_count < MAX_PLAYERS; i++) {
        char first_name[MAX_NAME_LENGTH], last_name[MAX_NAME_LENGTH];
        strcpy(first_name, first_names[rand() % 24]);
        strcpy(last_name, last_names[rand() % 18]);
        
        int age = 18 + (rand() % 20);
        double height = 1.65 + (rand() % 35) / 100.0;
        double weight = 60.0 + (rand() % 40);
        int team_id = 1 + (rand() % 30);
        int jersey_number = 1 + (rand() % 99);
        const char* position = positions[rand() % 4];
        const char* nationality = nationalities[rand() % 10];
        double market_value = 0.5 + (rand() % 100) + (rand() % 100) / 100.0;
        
        int contract_exp_year = 2024 + (rand() % 5);
        int contract_exp_month = 1 + (rand() % 12);
        int contract_exp_day = 1 + (rand() % 28);
        
        add_player(db, first_name, last_name, position, age, height, weight,
                  team_id, jersey_number, nationality, market_value,
                  contract_exp_day, contract_exp_month, contract_exp_year);
    }
    
    // Add games
    for (int week = 1; week <= 20; week++) {
        for (int league_id = 1; league_id <= 3; league_id++) {
            int teams_in_league = 0;
            int team_ids[MAX_TEAMS];
            
            // Find teams in this league
            for (int i = 0; i < db->team_count; i++) {
                if (db->teams[i].league_id == league_id && db->teams[i].active) {
                    team_ids[teams_in_league++] = db->teams[i].id;
                }
            }
            
            // Create fixtures (simplified - just random pairs)
            for (int i = 0; i < teams_in_league / 2 && db->game_count < MAX_GAMES; i++) {
                int home_team = team_ids[rand() % teams_in_league];
                int away_team = team_ids[rand() % teams_in_league];
                
                // Make sure they're not the same team
                while (away_team == home_team) {
                    away_team = team_ids[rand() % teams_in_league];
                }
                
                int day = 1 + (rand() % 28);
                int month = league_id == 1 ? (8 + (week % 8)) % 12 + 1 : (7 + (week % 6)) % 12 + 1;
                int year = 2023;
                
                if (month < 8) year++;
                
                char time[10];
                sprintf(time, "%02d:%02d", 14 + (rand() % 6), rand() % 60);
                
                int home_team_index = find_team_by_id(db, home_team);
                add_game(db, home_team, away_team, day, month, year, time, league_id, week,
                         home_team_index != -1 ? db->teams[home_team_index].stadium : "Stadium");
            }
        }
    }
    
    // Simulate some games
    simulate_games(db, 1, 30);
}

const char* get_position_description(const char *position) {
    if (strcmp(position, "Forward") == 0) return "Responsible for scoring goals and attacking play";
    if (strcmp(position, "Midfielder") == 0) return "Controls the midfield and links defense with attack";
    if (strcmp(position, "Defender") == 0) return "Responsible for preventing opposition attacks";
    if (strcmp(position, "Goalkeeper") == 0) return "Last line of defense, prevents goals";
    return "Unknown position";
}