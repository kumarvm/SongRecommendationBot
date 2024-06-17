const express = require('express');
const {exec} = require('child_process');
const cors = require('cors');
const app = express();

app.use(express.json());
app.use(cors());


app.post('/run', (req, res) => {
    const {track, artist} = req.body;
    if(!track || !artist) {
        return res.send('Song and artist both required.').status(400);
    }

    exec(`./Chatbot "${track.trim()}" "${artist.trim()}"`, (error, stdout) => {
        if (error) {
            return res.send('Song not found. Make sure both track and artist are correctly spelt. Otherwise, select \'Different Artist\' but note that the recommendations may not be by the same artist.').status(500);
        }
        res.send(stdout);
    });
});

if (process.env.DEVELOPMENT) {
    app.listen(3001, () => {
        console.log('Server running on port 3001');
    });
}