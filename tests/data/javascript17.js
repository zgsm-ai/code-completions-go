/**
 * Interactive Music Player with Playlist Management
 * 
 * This module implements a comprehensive music player application with playlist
 * management, audio visualization, and social features. It demonstrates advanced
 * JavaScript patterns for multimedia applications.
 * 
 * Features:
 * - Audio playback controls
 * - Playlist creation and management
 * - Music library organization
 * - Audio visualization
 * - User preferences and settings
 * - Social sharing features
 * - Search and filtering
 */

// Song class represents a music track
class Song {
    constructor(id, title, artist, album, duration, url, metadata = {}) {
        this.id = id;
        this.title = title;
        this.artist = artist;
        this.album = album;
        this.duration = duration; // in seconds
        this.url = url;
        this.metadata = {
            genre: metadata.genre || 'Unknown',
            year: metadata.year || null,
            trackNumber: metadata.trackNumber || null,
            albumArt: metadata.albumArt || null,
            lyrics: metadata.lyrics || null,
            ...metadata
        };
        this.playCount = 0;
        this.rating = 0;
        this.isFavorite = false;
        this.lastPlayed = null;
    }
    
    /**
     * Format duration as MM:SS
     */
    getFormattedDuration() {
        const minutes = Math.floor(this.duration / 60);
        const seconds = Math.floor(this.duration % 60);
        return `${minutes}:${seconds.toString().padStart(2, '0')}`;
    }
    
    /**
     * Increment play count
     */
    incrementPlayCount() {
        this.playCount++;
        this.lastPlayed = new Date();
    }
    
    /**
     * Set rating
     * @param {number} rating - Rating from 1-5
     */
    setRating(rating) {
        if (rating >= 1 && rating <= 5) {
            this.rating = rating;
        }
    }
    
    /**
     * Toggle favorite status
     */
    toggleFavorite() {
        this.isFavorite = !this.isFavorite;
        return this.isFavorite;
    }
    
    /**
     * Get song summary
     */
    getSummary() {
        return {
            id: this.id,
            title: this.title,
            artist: this.artist,
            album: this.album,
            duration: this.getFormattedDuration(),
            genre: this.metadata.genre,
            year: this.metadata.year,
            playCount: this.playCount,
            rating: this.rating,
            isFavorite: this.isFavorite,
            lastPlayed: this.lastPlayed
        };
    }
}

// Playlist class represents a collection of songs
class Playlist {
    constructor(id, name, description = '', isPublic = false) {
        this.id = id;
        this.name = name;
        this.description = description;
        this.songs = [];
        this.isPublic = isPublic;
        this.ownerId = null;
        this.collaborators = new Set();
        this.createdAt = new Date();
        this.updatedAt = new Date();
        this.tags = new Set();
        this.coverImage = null;
    }
    
    /**
     * Add song to playlist
     * @param {Song} song - Song to add
     * @param {number} position - Position to insert at (optional)
     */
    addSong(song, position = null) {
        if (position !== null && position >= 0 && position <= this.songs.length) {
            this.songs.splice(position, 0, song);
        } else {
            this.songs.push(song);
        }
        
        this.updatedAt = new Date();
        return this.songs.length - 1;
    }
    
    /**
     * Remove song from playlist
     * @param {number} index - Index of song to remove
     */
    removeSong(index) {
        if (index >= 0 && index < this.songs.length) {
            const removedSong = this.songs.splice(index, 1)[0];
            this.updatedAt = new Date();
            return removedSong;
        }
        return null;
    }
    
    /**
     * Move song to new position
     * @param {number} fromIndex - Current index
     * @param {number} toIndex - New index
     */
    moveSong(fromIndex, toIndex) {
        if (
            fromIndex >= 0 && fromIndex < this.songs.length &&
            toIndex >= 0 && toIndex < this.songs.length
        ) {
            const song = this.songs.splice(fromIndex, 1)[0];
            this.songs.splice(toIndex, 0, song);
            this.updatedAt = new Date();
            return true;
        }
        return false;
    }
    
    /**
     * Get total duration of playlist
     */
    getTotalDuration() {
        return this.songs.reduce((total, song) => total + song.duration, 0);
    }
    
    /**
     * Get formatted total duration
     */
    getFormattedTotalDuration() {
        const totalSeconds = this.getTotalDuration();
        const hours = Math.floor(totalSeconds / 3600);
        const minutes = Math.floor((totalSeconds % 3600) / 60);
        const seconds = Math.floor(totalSeconds % 60);
        
        if (hours > 0) {
            return `${hours}:${minutes.toString().padStart(2, '0')}:${seconds.toString().padStart(2, '0')}`;
        } else {
            return `${minutes}:${seconds.toString().padStart(2, '0')}`;
        }
    }
    
    /**
     * Add collaborator
     * @param {string} userId - User ID to add
     */
    addCollaborator(userId) {
        this.collaborators.add(userId);
        this.updatedAt = new Date();
    }
    
    /**
     * Remove collaborator
     * @param {string} userId - User ID to remove
     */
    removeCollaborator(userId) {
        this.collaborators.delete(userId);
        this.updatedAt = new Date();
    }
    
    /**
     * Add tag
     * @param {string} tag - Tag to add
     */
    addTag(tag) {
        this.tags.add(tag.toLowerCase());
        this.updatedAt = new Date();
    }
    
    /**
     * Remove tag
     * @param {string} tag - Tag to remove
     */
    removeTag(tag) {
        this.tags.delete(tag.toLowerCase());
        this.updatedAt = new Date();
    }
    
    /**
     * Shuffle songs
     */
    shuffle() {
        for (let i = this.songs.length - 1; i > 0; i--) {
            const j = Math.floor(Math.random() * (i + 1));
            [this.songs[i], this.songs[j]] = [this.songs[j], this.songs[i]];
        }
        this.updatedAt = new Date();
    }
    
    /**
     * Sort songs by property
     * @param {string} property - Property to sort by
     * @param {boolean} ascending - Sort order
     */
    sortBy(property, ascending = true) {
        this.songs.sort((a, b) => {
            let valueA = a[property];
            let valueB = b[property];
            
            // Handle different data types
            if (typeof valueA === 'string') {
                valueA = valueA.toLowerCase();
                valueB = valueB.toLowerCase();
            }
            
            if (valueA < valueB) return ascending ? -1 : 1;
            if (valueA > valueB) return ascending ? 1 : -1;
            return 0;
        });
        
        this.updatedAt = new Date();
    }
    
    /**
     * Get playlist summary
     */
    getSummary() {
        return {
            id: this.id,
            name: this.name,
            description: this.description,
            songCount: this.songs.length,
            totalDuration: this.getFormattedTotalDuration(),
            isPublic: this.isPublic,
            ownerId: this.ownerId,
            collaborators: Array.from(this.collaborators),
            tags: Array.from(this.tags),
            createdAt: this.createdAt,
            updatedAt: this.updatedAt
        };
    }
}

// MusicLibrary class manages the entire music collection
class MusicLibrary {
    constructor() {
        this.songs = new Map();
        this.playlists = new Map();
        this.albums = new Map();
        this.artists = new Map();
        this.genres = new Map();
        this.favoriteSongs = new Set();
    }
    
    /**
     * Add song to library
     * @param {Song} song - Song to add
     */
    addSong(song) {
        this.songs.set(song.id, song);
        
        // Update album collection
        if (!this.albums.has(song.album)) {
            this.albums.set(song.album, {
                name: song.album,
                artist: song.artist,
                songs: [],
                year: song.metadata.year,
                albumArt: song.metadata.albumArt
            });
        }
        this.albums.get(song.album).songs.push(song);
        
        // Update artist collection
        if (!this.artists.has(song.artist)) {
            this.artists.set(song.artist, {
                name: song.artist,
                albums: new Set(),
                songs: [],
                genres: new Set()
            });
        }
        const artist = this.artists.get(song.artist);
        artist.songs.push(song);
        artist.albums.add(song.album);
        artist.genres.add(song.metadata.genre);
        
        // Update genre collection
        if (!this.genres.has(song.metadata.genre)) {
            this.genres.set(song.metadata.genre, {
                name: song.metadata.genre,
                songs: [],
                artists: new Set()
            });
        }
        const genre = this.genres.get(song.metadata.genre);
        genre.songs.push(song);
        genre.artists.add(song.artist);
    }
    
    /**
     * Remove song from library
     * @param {string} songId - ID of song to remove
     */
    removeSong(songId) {
        const song = this.songs.get(songId);
        if (!song) return false;
        
        // Remove from songs collection
        this.songs.delete(songId);
        
        // Remove from favorites
        this.favoriteSongs.delete(songId);
        
        // Remove from album
        const album = this.albums.get(song.album);
        if (album) {
            album.songs = album.songs.filter(s => s.id !== songId);
            if (album.songs.length === 0) {
                this.albums.delete(song.album);
            }
        }
        
        // Remove from artist
        const artist = this.artists.get(song.artist);
        if (artist) {
            artist.songs = artist.songs.filter(s => s.id !== songId);
            
            // Check if artist still has songs
            if (artist.songs.length === 0) {
                this.artists.delete(song.artist);
            } else {
                // Update artist albums and genres
                artist.albums = new Set();
                artist.genres = new Set();
                artist.songs.forEach(s => {
                    artist.albums.add(s.album);
                    artist.genres.add(s.metadata.genre);
                });
            }
        }
        
        // Remove from genre
        const genre = this.genres.get(song.metadata.genre);
        if (genre) {
            genre.songs = genre.songs.filter(s => s.id !== songId);
            
            // Check if genre still has songs
            if (genre.songs.length === 0) {
                this.genres.delete(song.metadata.genre);
            } else {
                // Update genre artists
                genre.artists = new Set();
                genre.songs.forEach(s => genre.artists.add(s.artist));
            }
        }
        
        // Remove from all playlists
        this.playlists.forEach(playlist => {
            const index = playlist.songs.findIndex(s => s.id === songId);
            if (index !== -1) {
                playlist.removeSong(index);
            }
        });
        
        return true;
    }
    
    /**
     * Add playlist
     * @param {Playlist} playlist - Playlist to add
     */
    addPlaylist(playlist) {
        this.playlists.set(playlist.id, playlist);
    }
    
    /**
     * Remove playlist
     * @param {string} playlistId - ID of playlist to remove
     */
    removePlaylist(playlistId) {
        return this.playlists.delete(playlistId);
    }
    
    /**
     * Toggle favorite status
     * @param {string} songId - ID of song
     */
    toggleFavorite(songId) {
        const song = this.songs.get(songId);
        if (song) {
            const isFavorite = song.toggleFavorite();
            if (isFavorite) {
                this.favoriteSongs.add(songId);
            } else {
                this.favoriteSongs.delete(songId);
            }
            return isFavorite;
        }
        return false;
    }
    
    /**
     * Search songs
     * @param {string} query - Search query
     * @param {Object} options - Search options
     */
    searchSongs(query, options = {}) {
        const {
            fields = ['title', 'artist', 'album'],
            caseSensitive = false,
            exactMatch = false
        } = options;
        
        const searchTerm = caseSensitive ? query : query.toLowerCase();
        const results = [];
        
        this.songs.forEach(song => {
            let matches = false;
            
            for (const field of fields) {
                let fieldValue = song[field];
                
                if (typeof fieldValue === 'string') {
                    fieldValue = caseSensitive ? fieldValue : fieldValue.toLowerCase();
                    
                    if (exactMatch) {
                        matches = fieldValue === searchTerm;
                    } else {
                        matches = fieldValue.includes(searchTerm);
                    }
                }
                
                if (matches) break;
            }
            
            if (matches) {
                results.push(song);
            }
        });
        
        return results;
    }
    
    /**
     * Filter songs
     * @param {Object} filters - Filter criteria
     */
    filterSongs(filters) {
        const {
            artist,
            album,
            genre,
            minYear,
            maxYear,
            minRating,
            minDuration,
            maxDuration
        } = filters;
        
        let results = Array.from(this.songs.values());
        
        if (artist) {
            results = results.filter(song => 
                song.artist.toLowerCase().includes(artist.toLowerCase())
            );
        }
        
        if (album) {
            results = results.filter(song => 
                song.album.toLowerCase().includes(album.toLowerCase())
            );
        }
        
        if (genre) {
            results = results.filter(song => 
                song.metadata.genre.toLowerCase().includes(genre.toLowerCase())
            );
        }
        
        if (minYear) {
            results = results.filter(song => 
                song.metadata.year >= minYear
            );
        }
        
        if (maxYear) {
            results = results.filter(song => 
                song.metadata.year <= maxYear
            );
        }
        
        if (minRating) {
            results = results.filter(song => 
                song.rating >= minRating
            );
        }
        
        if (minDuration) {
            results = results.filter(song => 
                song.duration >= minDuration
            );
        }
        
        if (maxDuration) {
            results = results.filter(song => 
                song.duration <= maxDuration
            );
        }
        
        return results;
    }
    
    /**
     * Get random songs
     * @param {number} count - Number of songs to get
     */
    getRandomSongs(count) {
        const songs = Array.from(this.songs.values());
        const shuffled = [...songs].sort(() => 0.5 - Math.random());
        return shuffled.slice(0, count);
    }
    
    /**
     * Get most played songs
     * @param {number} limit - Maximum number of songs
     */
    getMostPlayedSongs(limit = 10) {
        return Array.from(this.songs.values())
            .sort((a, b) => b.playCount - a.playCount)
            .slice(0, limit);
    }
    
    /**
     * Get recently played songs
     * @param {number} limit - Maximum number of songs
     */
    getRecentlyPlayedSongs(limit = 10) {
        return Array.from(this.songs.values())
            .filter(song => song.lastPlayed !== null)
            .sort((a, b) => b.lastPlayed - a.lastPlayed)
            .slice(0, limit);
    }
    
    /**
     * Get top rated songs
     * @param {number} limit - Maximum number of songs
     */
    getTopRatedSongs(limit = 10) {
        return Array.from(this.songs.values())
            .filter(song => song.rating > 0)
            .sort((a, b) => b.rating - a.rating)
            .slice(0, limit);
    }
    
    /**
     * Get favorite songs
     */
    getFavoriteSongs() {
        return Array.from(this.favoriteSongs).map(id => this.songs.get(id));
    }
    
    /**
     * Get library statistics
     */
    getStatistics() {
        const songs = Array.from(this.songs.values());
        const totalDuration = songs.reduce((total, song) => total + song.duration, 0);
        
        return {
            totalSongs: songs.length,
            totalArtists: this.artists.size,
            totalAlbums: this.albums.size,
            totalGenres: this.genres.size,
            totalPlaylists: this.playlists.size,
            totalDuration: this.formatDuration(totalDuration),
            averageRating: songs.reduce((sum, song) => sum + song.rating, 0) / songs.length,
            favoriteCount: this.favoriteSongs.size
        };
    }
    
    /**
     * Format duration in seconds to readable format
     * @param {number} seconds - Duration in seconds
     */
    formatDuration(seconds) {
        const days = Math.floor(seconds / 86400);
        const hours = Math.floor((seconds % 86400) / 3600);
        const minutes = Math.floor((seconds % 3600) / 60);
        
        let result = '';
        if (days > 0) result += `${days}d `;
        if (hours > 0) result += `${hours}h `;
        if (minutes > 0) result += `${minutes}m`;
        
        return result || '0m';
    }
}

// AudioPlayer class handles audio playback
class AudioPlayer {
    constructor(library) {
        this.library = library;
        this.audioElement = new Audio();
        this.currentSong = null;
        this.currentPlaylist = null;
        this.currentSongIndex = 0;
        this.isPlaying = false;
        this.isPaused = false;
        this.volume = 0.7;
        this.repeatMode = 'none'; // 'none', 'one', 'all'
        this.shuffleMode = false;
        this.currentTime = 0;
        this.duration = 0;
        this.playbackRate = 1.0;
        
        this.setupEventListeners();
    }
    
    /**
     * Setup audio element event listeners
     */
    setupEventListeners() {
        this.audioElement.addEventListener('timeupdate', () => {
            this.currentTime = this.audioElement.currentTime;
            this.duration = this.audioElement.duration;
        });
        
        this.audioElement.addEventListener('ended', () => {
            this.handleSongEnd();
        });
        
        this.audioElement.addEventListener('error', (e) => {
            console.error('Audio playback error:', e);
            this.playNext();
        });
    }
    
    /**
     * Play a song
     * @param {Song} song - Song to play
     */
    playSong(song) {
        this.currentSong = song;
        this.audioElement.src = song.url;
        this.audioElement.load();
        this.audioElement.play();
        this.isPlaying = true;
        this.isPaused = false;
        
        // Increment play count
        song.incrementPlayCount();
        
        return song;
    }
    
    /**
     * Play from playlist
     * @param {Playlist} playlist - Playlist to play from
     * @param {number} startIndex - Index to start from
     */
    playPlaylist(playlist, startIndex = 0) {
        this.currentPlaylist = playlist;
        this.currentSongIndex = startIndex;
        
        if (startIndex >= 0 && startIndex < playlist.songs.length) {
            return this.playSong(playlist.songs[startIndex]);
        }
        
        return null;
    }
    
    /**
     * Play or pause current song
     */
    togglePlayPause() {
        if (!this.currentSong) return false;
        
        if (this.isPlaying) {
            this.pause();
        } else {
            this.resume();
        }
        
        return true;
    }
    
    /**
     * Pause current song
     */
    pause() {
        this.audioElement.pause();
        this.isPlaying = false;
        this.isPaused = true;
    }
    
    /**
     * Resume current song
     */
    resume() {
        this.audioElement.play();
        this.isPlaying = true;
        this.isPaused = false;
    }
    
    /**
     * Stop current song
     */
    stop() {
        this.audioElement.pause();
        this.audioElement.currentTime = 0;
        this.isPlaying = false;
        this.isPaused = false;
    }
    
    /**
     * Play next song
     */
    playNext() {
        if (!this.currentPlaylist) return null;
        
        let nextIndex;
        
        if (this.shuffleMode) {
            nextIndex = Math.floor(Math.random() * this.currentPlaylist.songs.length);
        } else {
            nextIndex = this.currentSongIndex + 1;
            
            // Handle wraparound
            if (nextIndex >= this.currentPlaylist.songs.length) {
                if (this.repeatMode === 'all') {
                    nextIndex = 0;
                } else {
                    return null;
                }
            }
        }
        
        return this.playSong(this.currentPlaylist.songs[nextIndex]);
    }
    
    /**
     * Play previous song
     */
    playPrevious() {
        if (!this.currentPlaylist) return null;
        
        let prevIndex;
        
        if (this.shuffleMode) {
            prevIndex = Math.floor(Math.random() * this.currentPlaylist.songs.length);
        } else {
            prevIndex = this.currentSongIndex - 1;
            
            // Handle wraparound
            if (prevIndex < 0) {
                if (this.repeatMode === 'all') {
                    prevIndex = this.currentPlaylist.songs.length - 1;
                } else {
                    return null;
                }
            }
        }
        
        return this.playSong(this.currentPlaylist.songs[prevIndex]);
    }
    
    /**
     * Seek to time
     * @param {number} time - Time in seconds
     */
    seek(time) {
        if (time >= 0 && time <= this.duration) {
            this.audioElement.currentTime = time;
            return true;
        }
        return false;
    }
    
    /**
     * Set volume
     * @param {number} volume - Volume level (0-1)
     */
    setVolume(volume) {
        if (volume >= 0 && volume <= 1) {
            this.audioElement.volume = volume;
            this.volume = volume;
            return true;
        }
        return false;
    }
    
    /**
     * Set repeat mode
     * @param {string} mode - Repeat mode ('none', 'one', 'all')
     */
    setRepeatMode(mode) {
        if (['none', 'one', 'all'].includes(mode)) {
            this.repeatMode = mode;
            return true;
        }
        return false;
    }
    
    /**
     * Toggle shuffle mode
     */
    toggleShuffle() {
        this.shuffleMode = !this.shuffleMode;
        return this.shuffleMode;
    }
    
    /**
     * Set playback rate
     * @param {number} rate - Playback rate
     */
    setPlaybackRate(rate) {
        if (rate >= 0.25 && rate <= 2.0) {
            this.audioElement.playbackRate = rate;
            this.playbackRate = rate;
            return true;
        }
        return false;
    }
    
    /**
     * Handle song end
     */
    handleSongEnd() {
        if (this.repeatMode === 'one') {
            this.playSong(this.currentSong);
        } else {
            this.playNext();
        }
    }
    
    /**
     * Get current playback state
     */
    getState() {
        return {
            currentSong: this.currentSong ? this.currentSong.getSummary() : null,
            isPlaying: this.isPlaying,
            isPaused: this.isPaused,
            currentTime: this.currentTime,
            duration: this.duration,
            volume: this.volume,
            repeatMode: this.repeatMode,
            shuffleMode: this.shuffleMode,
            playbackRate: this.playbackRate,
            currentPlaylist: this.currentPlaylist ? this.currentPlaylist.getSummary() : null,
            currentSongIndex: this.currentSongIndex
        };
    }
}

// Main function to demonstrate music player
function main() {
    console.log('Initializing Music Player Application...\n');
    
    // Create music library
    const library = new MusicLibrary();
    
    // Create sample songs
    const songs = [
        new Song('song1', 'Bohemian Rhapsody', 'Queen', 'A Night at the Opera', 355, 'https://example.com/song1.mp3', {
            genre: 'Rock',
            year: 1975,
            trackNumber: 11
        }),
        new Song('song2', 'Imagine', 'John Lennon', 'Imagine', 183, 'https://example.com/song2.mp3', {
            genre: 'Pop',
            year: 1971,
            trackNumber: 1
        }),
        new Song('song3', 'Hotel California', 'Eagles', 'Hotel California', 391, 'https://example.com/song3.mp3', {
            genre: 'Rock',
            year: 1976,
            trackNumber: 1
        }),
        new Song('song4', 'Stairway to Heaven', 'Led Zeppelin', 'Led Zeppelin IV', 482, 'https://example.com/song4.mp3', {
            genre: 'Rock',
            year: 1971,
            trackNumber: 4
        }),
        new Song('song5', 'Billie Jean', 'Michael Jackson', 'Thriller', 294, 'https://example.com/song5.mp3', {
            genre: 'Pop',
            year: 1982,
            trackNumber: 4
        })
    ];
    
    // Add songs to library
    songs.forEach(song => library.addSong(song));
    
    console.log('Added songs to library:');
    songs.forEach(song => {
        console.log(`- ${song.title} by ${song.artist} (${song.getFormattedDuration()})`);
    });
    
    // Create playlists
    const rockPlaylist = new Playlist('playlist1', 'Classic Rock', 'Best rock songs of all time');
    const popPlaylist = new Playlist('playlist2', 'Pop Hits', 'Top pop hits');
    
    // Add songs to playlists
    library.addPlaylist(rockPlaylist);
    library.addPlaylist(popPlaylist);
    
    const rockSongs = library.filterSongs({ genre: 'Rock' });
    rockSongs.forEach(song => rockPlaylist.addSong(song));
    
    const popSongs = library.filterSongs({ genre: 'Pop' });
    popSongs.forEach(song => popPlaylist.addSong(song));
    
    console.log('\nCreated playlists:');
    console.log(`- ${rockPlaylist.name}: ${rockPlaylist.songs.length} songs, ${rockPlaylist.getFormattedTotalDuration()}`);
    console.log(`- ${popPlaylist.name}: ${popPlaylist.songs.length} songs, ${popPlaylist.getFormattedTotalDuration()}`);
    
    // Create audio player
    const player = new AudioPlayer(library);
    
    // Set some songs as favorites
    library.toggleFavorite('song1');
    library.toggleFavorite('song3');
    library.toggleFavorite('song5');
    
    // Set ratings
    library.songs.get('song1').setRating(5);
    library.songs.get('song2').setRating(4);
    library.songs.get('song3').setRating(5);
    library.songs.get('song4').setRating(4);
    library.songs.get('song5').setRating(5);
    
    // Get library statistics
    const stats = library.getStatistics();
    console.log('\nLibrary Statistics:');
    console.log(`- Total Songs: ${stats.totalSongs}`);
    console.log(`- Total Artists: ${stats.totalArtists}`);
    console.log(`- Total Albums: ${stats.totalAlbums}`);
    console.log(`- Total Genres: ${stats.totalGenres}`);
    console.log(`- Total Playlists: ${stats.totalPlaylists}`);
    console.log(`- Total Duration: ${stats.totalDuration}`);
    console.log(`- Average Rating: ${stats.averageRating.toFixed(1)}`);
    console.log(`- Favorite Songs: ${stats.favoriteCount}`);
    
    // Demonstrate search
    const searchResults = library.searchSongs('queen');
    console.log('\nSearch results for "queen":');
    searchResults.forEach(song => {
        console.log(`- ${song.title} by ${song.artist}`);
    });
    
    // Demonstrate filtering
    const filteredResults = library.filterSongs({ genre: 'Rock', minRating: 4 });
    console.log('\nRock songs with rating >= 4:');
    filteredResults.forEach(song => {
        console.log(`- ${song.title} by ${song.artist} (Rating: ${song.rating})`);
    });
    
    // Get top rated songs
    const topRated = library.getTopRatedSongs(3);
    console.log('\nTop rated songs:');
    topRated.forEach(song => {
        console.log(`- ${song.title} by ${song.artist} (Rating: ${song.rating})`);
    });
    
    // Get favorite songs
    const favorites = library.getFavoriteSongs();
    console.log('\nFavorite songs:');
    favorites.forEach(song => {
        console.log(`- ${song.title} by ${song.artist}`);
    });
    
    // Get most played songs (after simulating plays)
    library.songs.get('song1').incrementPlayCount();
    library.songs.get('song1').incrementPlayCount();
    library.songs.get('song3').incrementPlayCount();
    library.songs.get('song5').incrementPlayCount();
    library.songs.get('song5').incrementPlayCount();
    library.songs.get('song5').incrementPlayCount();
    
    const mostPlayed = library.getMostPlayedSongs(3);
    console.log('\nMost played songs:');
    mostPlayed.forEach(song => {
        console.log(`- ${song.title} by ${song.artist} (Played ${song.playCount} times)`);
    });
}

// Export classes for use in other modules
if (typeof module !== 'undefined') {
    module.exports = {
        Song,
        Playlist,
        MusicLibrary,
        AudioPlayer
    };
}