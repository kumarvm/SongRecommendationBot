import sqlite3
import csv

conn = sqlite3.connect("Songs.db")
c = conn.cursor()

c.execute('CREATE TABLE IF NOT EXISTS songs (id INTEGER PRIMARY KEY, track_name TEXT, artist TEXT, genre TEXT, acousticness REAL, danceability REAL, energy REAL, song_key TEXT, liveness REAL, loudness REAL, mode TEXT, speechiness REAL, tempo REAL, valence REAL)')

with open('SpotifyFeatures.csv', newline='') as csvFile:
    reader = csv.reader(csvFile)
    next(reader)
    
    for row in reader:
        track_name = row[2]
        artist = row[1]
        genre = row[0]
        acousticness = row[5]
        danceability = row[6]
        energy = row[8]
        song_key = row[10]
        liveness = row[11]
        loudness = row[12]
        mode = row[13]
        speechiness = row[14]
        tempo = row[15]
        valence = row[17]
    
        c.execute("INSERT INTO songs (track_name, artist, genre, acousticness, danceability, energy, song_key, liveness, loudness, mode, speechiness, tempo, valence) VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?)", (track_name, artist, genre, acousticness, danceability, energy, song_key, liveness, loudness, mode, speechiness, tempo, valence))

conn.commit()
conn.close()    