# SongRecommendationBot

## Overview
A website which recommends songs based a seed song inputted by the user.

## Description
The website takes in a seed song and artist and then, based on the user's preference on whether they want recommendations from the same or a different artist, 5 recommendations are returned.

The project utilises a different method to generate recommendations for each type of recommendation.

### Same Artist
This preference requires the use of a SQLite3 database and C++ file. Firstly the database is created and filled by using Python to read a CSV file (link to Kaggle dataset found below). Then the recommendations are generated by the C++ code, through implementation of the k-nearest neighbours algorithm. This algorithm works by storing each song and its metrics to a vector and then finding the Euclidean distance between the seed vector and every other vector. The k smallest distances are the songs which have the most similar metrics to the seed vector and so are the k-nearest neighbours. The C++ backend file is called in server/server.js via the exec function from child_process and the stdout from the program call is then sent to the React app for display.

### Different Artist
This preference utilises the Spotify Web API and its function called getRecommendations. The code for connecting to the API and doing the API request can be found in client/spotifyAPI.js.

## How to install and run the project
Firstly open the project in the terminal (or IDE) and make sure all dependencies are installed. To do this, run the dependencies shell script:

```bash
./dependencies.sh
```

Note that you only have to run the dependencies script once ie. when you have initially forked this repo.

After that, run the run_server shell script to start the server:
```bash
./run_server.sh
```

Once the server is running, click the link found with this repo.

(To stop the server, type Ctrl+C in the terminal.)

## Space for improvements
AWS Amplify was used to deploy the React app, however currently the Node.js server is not deployed. AWS EC2, AWS Lambda and AWS Elastic Beanstalk can be used to try and deploy the server however take care to note when the server needs to be converted to serverless (as is the case for AWS Lambda).

Additional features to the website can be added: same album recommendations, same/different genres, recommendations based on the metrics found in the CSV file (and the API).

## Links
Website (deployed on AWS Amplify): https://main.d3mx11xvagsje9.amplifyapp.com/
Kaggle Dataset: https://www.kaggle.com/datasets/zaheenhamidani/ultimate-spotify-tracks-db/data
