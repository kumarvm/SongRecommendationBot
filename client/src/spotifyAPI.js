import axios from 'axios';

const SPOTIFY_CLIENT_ID = '9c90ab47a743489dbfef24be7d7a88ed';
const SPOTIFY_CLIENT_SECRET = '1f13ed6b307c459a95194e39692fcf8a';
const SPOTIFY_TOKEN_URL = 'https://accounts.spotify.com/api/token';
const SPOTIFY_SEARCH_URL = 'https://api.spotify.com/v1/search';
const SPOTIFY_RECOMMENDATIONS_URL = 'https://api.spotify.com/v1/recommendations';

let accessToken = '';
let tokenExpiryTime = 0;

const getAccessToken = async () => {
  if (!accessToken || Date.now() >= tokenExpiryTime) {
    const response = await axios.post(SPOTIFY_TOKEN_URL, 'grant_type=client_credentials', {
      headers: {
        'Content-Type': 'application/x-www-form-urlencoded',
        'Authorization': `Basic ${btoa(`${SPOTIFY_CLIENT_ID}:${SPOTIFY_CLIENT_SECRET}`)}`,
      },
    });
    accessToken = response.data.access_token;
    tokenExpiryTime = Date.now() + response.data.expires_in * 1000;
  }
};

const searchTrack = async (track, artist) => {
  await getAccessToken();
  const response = await axios.get(SPOTIFY_SEARCH_URL, {
    headers: {
      'Authorization': `Bearer ${accessToken}`,
    },
    params: {
      q: `track:${track} artist:${artist}`,
      type: 'track',
      limit: 1,
    },
  });
  return response.data.tracks.items[0];
};

const searchTrackID = async (track, artist) => {
    await getAccessToken();
    const response = await axios.get(SPOTIFY_SEARCH_URL, {
      headers: {
        'Authorization': `Bearer ${accessToken}`,
      },
      params: {
        q: `track:${track} artist:${artist}`,
        type: 'track',
        limit: 1,
      },
    });
    console.log(response.data)
    return response.data.tracks.items[0].id;
  };

const getRecommendations = async (seedTrack, artist, limit = 5) => {
    await getAccessToken();
    const seedTrackID = await searchTrackID(seedTrack, artist);
    const response = await axios.get(SPOTIFY_RECOMMENDATIONS_URL, {
      headers: {
        'Authorization': `Bearer ${accessToken}`,
      },
      params: {
        seed_tracks: seedTrackID,
        limit: limit,
      },
    });
    return response.data;
  };

export {searchTrack, getRecommendations};

  
