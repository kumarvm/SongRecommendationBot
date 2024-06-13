Chatbot: SongRecommendationChatbot.o
	g++ -o Chatbot SongRecommendationChatbot.o -lsqlite3

SongRecommendationChatbot.o: SongRecommendationChatbot.cpp
	g++ -c SongRecommendationChatbot.cpp

clean:
	rm -f SongRecommendationChatbot.o Chatbot