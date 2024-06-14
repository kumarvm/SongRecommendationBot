import React, {useState, useEffect} from 'react';
import axios from 'axios';
import './App.css';
import {searchTrack, getRecommendations} from './spotifyAPI.js';

function App() {
  var [track, setTrack] = useState('');
  var [artist, setArtist] = useState('');
  var [typeRec, setTypeRec] = useState('');
  const [response, setResponse] = useState('');
  const [songImage, setSongImage] = useState('');

  useEffect(() => {
    const fetchSongImage = async () => {
      if (track && artist) {
        try {
          const song = await searchTrack(track, artist);
          if (song && song.album && song.album.images.length > 0) {
            setSongImage(song.album.images[0].url);
          }
          else {
            setSongImage('');
          }
        }
        catch (error) {
          setSongImage('');
        }
      }
      else {
        setSongImage('');
      }
    };

    fetchSongImage();
  }, [track, artist]);


  const handleSubmit = async (event) => {
    event.preventDefault();
    try {
      if (!typeRec) {
        setResponse("Please select a type of recommendation.");
        return;
      }

      setResponse("Recommending...");
      if (typeRec === "Same") {
        const res = await axios.post("http://localhost:3000/run", {track, artist, typeRec});
        setResponse(res.data);
      }
      else if (typeRec === "Different") {
        const res = await getRecommendations(track, artist);
        const trackNames = res.tracks.map(track => track.name);
        const result = trackNames.join('\n');
        setResponse(result);
      }
    }
    catch (error) {
      setResponse('Error: ' + error.message);
      setSongImage('');
    }
  };

  return (
    <div>
      <h1>Song Recommendation Bot</h1>
      <p>Need some recommendations? 
        Simply enter the song you are listening to right now 
        and the artist and I will recommend other songs by that artist!

        <p>If you would like recommendations for songs by other artists, make sure to select 'Different Artist'.</p>
      </p>
      <form onSubmit={handleSubmit}>
        <input type="text" value={track} onChange={(e) => setTrack(e.target.value)} placeholder='Enter the song'/>
        <input type="text" value={artist} onChange={(e) => setArtist(e.target.value)} placeholder='Enter the artist'/>
        <select value={typeRec} onChange={(e) => setTypeRec(e.target.value)}>
          <option value="">Select type of recommendation</option>
          <option value="Same">Same Artist</option>
          <option value="Different">Different Artist</option>
        </select>
        <div class="image">
        {songImage && <img src={songImage} alt="Song album cover" style={{ marginLeft: '10px', width: '100px', height: '100px' }}/>}
        </div>
        <button type="submit">Send</button>
      </form>
      <div class="response">
      {response.split('\n').map((line, index) => (
      <p key={index}>{line}</p>
        ))}
      </div>
    </div>
  );
}

export default App;
