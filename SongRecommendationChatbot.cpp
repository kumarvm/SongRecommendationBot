#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>
#include <sqlite3.h>

using namespace std;

typedef struct Song {
    string track_name;
    string artist;
    string genre;
    double acousticness;
    double danceability; 
    double energy;
    string key; 
    double liveness;
    double loudness;
    string mode; 
    double speechiness;
    double tempo;
    double valence;
} Song;

void error_handler(int rc, string message, sqlite3* db) {
    if (rc) {
        cerr << message << endl;
        sqlite3_close(db);
        exit(1);
    }
}

double scale_tempo() {
    sqlite3* db;
    int rc = sqlite3_open("Songs.db", &db);
    error_handler(rc, "Error opening database.", db);

    sqlite3_stmt* stmt1;
    const char* query1 = "SELECT MAX(tempo) FROM Songs;";
    rc = sqlite3_prepare_v2(db, query1, -1, &stmt1, 0);
    error_handler(rc, "Error while preparing query.", db);

    sqlite3_stmt* stmt2;
    const char* query2 = "SELECT MIN(tempo) FROM Songs;";
    rc = sqlite3_prepare_v2(db, query2, -1, &stmt2, 0);
    error_handler(rc, "Error while preparingc query.", db);

    double max;
    double min;

    while ((rc = sqlite3_step(stmt1)) == SQLITE_ROW) {
        max = sqlite3_column_double(stmt1, 0);
    }

    while ((rc = sqlite3_step(stmt2)) == SQLITE_ROW) {
        min = sqlite3_column_double(stmt2, 0);
    }

    sqlite3_finalize(stmt1);
    sqlite3_finalize(stmt2);
    sqlite3_close(db);

    return max, min;
}

Song fill_data(Song song, sqlite3_stmt* stmt, double max, double min) {
    song.track_name = (const char*)(sqlite3_column_text(stmt, 1));
    song.artist = (const char*)(sqlite3_column_text(stmt, 2));
    song.genre = (const char*)(sqlite3_column_text(stmt, 3));
    song.acousticness = sqlite3_column_double(stmt, 4);
    song.danceability = sqlite3_column_double(stmt, 5);
    song.energy = sqlite3_column_double(stmt, 6);
    song.key = (const char*)(sqlite3_column_text(stmt, 7));
    song.liveness = sqlite3_column_double(stmt, 8);
    song.loudness = sqlite3_column_double(stmt, 9);
    song.mode = (const char*)(sqlite3_column_text(stmt, 10));
    song.speechiness = sqlite3_column_double(stmt, 11);
    song.tempo = (sqlite3_column_double(stmt, 12) - min) / (max - min);
    song.valence = sqlite3_column_double(stmt, 13);

    return song;
}

Song get_input_song(int argc, char* argv[], sqlite3* db) {
    string input_song = argv[1];
    string artist = argv[2];

    sqlite3_stmt* stmt;
    const char* query = "SELECT * FROM Songs WHERE track_name = ? AND artist = ? LIMIT 1;";
    
    int rc = sqlite3_prepare_v2(db, query, -1, &stmt, 0);
    error_handler(rc, "Error while preparing query.", db);

    rc = sqlite3_bind_text(stmt, 1, input_song.c_str(), -1, SQLITE_STATIC);
    error_handler(rc, "Error while constructing query.", db);

    rc = sqlite3_bind_text(stmt, 2, artist.c_str(), -1, SQLITE_STATIC);
    error_handler(rc, "Error while constructing query.", db);

    double max;
    double min;
    max, min = scale_tempo();

    Song song;
    if ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        song = fill_data(song, stmt, max, min);
    }
    else {
        cout << "Please enter a valid song." << endl;
        exit(1);
    }

    sqlite3_finalize(stmt);

    return song;
}

vector<Song> get_dataset(sqlite3* db) {
    sqlite3_stmt* stmt;
    const char* sql = "SELECT * FROM Songs;";
    
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    error_handler(rc, "Error while preparing query.", db);

    double max;
    double min; 
    max, min = scale_tempo();

    vector<Song> dataset;
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        Song song;
        song = fill_data(song, stmt, max, min);

        dataset.push_back(song);
    }
    
    sqlite3_finalize(stmt);

    return dataset;
}

double euclidean_distance(const Song s1, const Song s2) {
    double distance = 0.0;
    distance += pow(s1.acousticness - s2.acousticness, 2);
    distance += pow(s1.danceability - s2.danceability, 2);
    distance += pow(s1.energy - s2.energy, 2);
    distance += pow(s1.liveness - s2.liveness, 2);
    distance += pow(s1.loudness - s2.loudness, 2);
    distance += pow(s1.speechiness - s2.speechiness, 2);
    distance += pow(s1.tempo - s2.tempo, 2);
    distance += pow(s1.valence - s2.valence, 2);

    if (s1.genre != s2.genre) {
        distance += 0.5;
    }
    if (s1.key != s2.key) {
        distance += 0.5;
    }
    if (s1.mode != s2.mode) {
        distance += 0.5;
    }

    return sqrt(distance);
}

vector<string> recommend_songs(const Song input_song, const vector<Song> dataset, int k) {
    vector<pair<double, string>> distances;
    vector<string> seenTracks;

    for (Song song : dataset) {
        if (song.track_name != input_song.track_name && song.artist == input_song.artist) {
            double distance = euclidean_distance(input_song, song);
            
            if (find(seenTracks.begin(), seenTracks.end(), song.track_name) == seenTracks.end()) {
                distances.push_back({distance, song.track_name});
                seenTracks.push_back(song.track_name);
            }
        }
    }

    sort(distances.begin(), distances.end());

    vector<string> recommendations;
    for (int i = 0; i < k && i < distances.size(); i++) {
        recommendations.push_back(distances[i].second);
    }

    return recommendations;
}

int main(int argc, char* argv[]) {
    sqlite3* db;
    int rc = sqlite3_open("Songs.db", &db);
    error_handler(rc, "Error opening database.", db);

    Song song = get_input_song(argc, argv, db);
    vector<Song> dataset = get_dataset(db);
    vector<string> recommendations = recommend_songs(song, dataset, 5);

    for (string song : recommendations) {
        cout << song << endl;
    }

    sqlite3_close(db);

    return 0;
}