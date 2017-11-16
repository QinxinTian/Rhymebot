#include "RhymeBot.h"


/** Inserting Words **/

void RhymeBot::insertWithPronunciation(string word, vector<Sound> pronunciation) {
	// TODO
	soundMap[word] = pronunciation;
	// The following lines should be removed. They are here to show what is being added to you data structures

	/*
	cout << word << ": ";
	for(int i=0; i<pronunciation.size(); i++){
		cout << pronunciation.at(i) << " ";
	}

	cout << endl;*/

}

void RhymeBot::addPartOfSpeech(string word, PART_OF_SPEECH partOfSpeech) {
	// TODO
	if (part_of_speech_map.find(partOfSpeech) != part_of_speech_map.end()) {
		part_of_speech_map[partOfSpeech].insert(word);
	}
	else {
		set<string> temp;
		temp.insert(word);
		part_of_speech_map[partOfSpeech] = temp;
	}
	// The following line should be removed. They are here to show what is being added to you data structures
	//cout << word << ": " << partOfSpeech << endl;

}


/** Helper Functions **/

bool RhymeBot::endsWith(string word, string s) {
	if (word.size() < s.size()) {
		return false;
	}
	string sub = word.substr(word.size() - s.size());
	cout << sub << endl;
	return sub.compare(s) == 0;
}


// returns the number of syllables in word
int RhymeBot::countSyllables(string word) {
	vector<Sound> sound;
	if (soundMap.find(word) != soundMap.end()) {
		sound = soundMap.at(word);
	}
	else {
		return 0;
	}
	int count = 0;
	for (int i = 0; i < sound.size(); i++) {
		if (sound.at(i).isVowel()) {
			count++;
		}
	}
	return count;

}

// returns the number of rhyming syllables in the two words. This is defined as the number of vowel sounds in the
//  largest rhyming suffix.
//
// Words are considered a rhyme if they have the same last vowel sound as well as the same sounds following
//  the last vowel. Note: These are syllabic rhymes since the matching vowels might not both be stressed
int RhymeBot::rhymingSyllables(string word1, string word2) {
	if (soundMap.find(word1) == soundMap.end() || soundMap.find(word2) == soundMap.end()) {
		return 0;
	}
	vector<Sound> sound1 = soundMap.at(word1);
	vector<Sound> sound2 = soundMap.at(word2);
	int index1 = sound1.size() - 1;
	int index2 = sound2.size() - 1;
	int count = 0;
	while (index1 >= 0 && index2 >= 0) {
		string sound_of_sound1 = sound1.at(index1).getSourceString();
		string sound_of_sound2 = sound2.at(index2).getSourceString();
		if (sound_of_sound1.compare(sound_of_sound2) == 0) {
			count++;
			index1--;
			index2--;
		}
		else if (sound1.at(index1).isVowel() || sound2.at(index2).isVowel()) {
			return count;
		}
	}
	return count;
}

// returns the number of identical sounds in the largest matching prefix of the two words.
//
// Words are considered an alliteration if they have the same first sound
int RhymeBot::alliterationSounds(string word1, string word2) {

	if (soundMap.find(word1) == soundMap.end() || soundMap.find(word2) == soundMap.end()) {
		return 0;
	}
	vector<Sound> sound1 = soundMap.at(word1);
	vector<Sound> sound2 = soundMap.at(word2);
	int index1 = 0;
	int index2 = 0;
	int count = 0;
	while (index1 <= sound1.size() - 1 && index2 <= sound2.size() - 1) {
		string string1 = sound1.at(index1).getSourceString();
		string string2 = sound2.at(index2).getSourceString();
		if (string1.compare(string2) == 0) {
			count++;
			index1++;
			index2++;
		}
		else if (string1.compare(string2) != 0) {
			return count;
		}
	}
	return count;
}


/** Search for Rhymes and Alliterations **/

// returns all words that rhyme at least 1 syllable with the input word
// returns the empty vector if no rhymes are found or if the given word is not in the dictionary
vector<string>* RhymeBot::getAllRhymes(string word) {
	vector<string>* rhymingWords = new vector<string>;
	// TODO
	map<string, vector<Sound>>::iterator it = soundMap.begin();

	while (it != soundMap.end()) {
		if (rhymingSyllables(it->first, word) > 0) {
			rhymingWords->push_back(it->first);
		}
		it++;
	}
	return rhymingWords;
}

// returns all words that form an alliteration of at least 1 sound with the input word
// returns the empty vector if no rhymes are found or the given word is not known
vector<string>* RhymeBot::getAllAlliterations(string word) {
	vector<string>* alliterationWords = new vector<string>;
	// TODO

	map<string, vector<Sound>>::iterator it = soundMap.begin();

	while (it != soundMap.end()) {
		if (alliterationSounds(it->first, word) > 0) {
			alliterationWords->push_back(it->first);
		}
		it++;
	}

	return alliterationWords;
}


/** Finding the Perfect Word **/

// returns the best word with the specified number of syllables and part of speech that rhymes with the given word.
// Best is defined by the maximum number of rhyming syllables.
string RhymeBot::findBestRhyme(int numberOfSyllables, string rhymesWith) {
	// TODO
	map<int, string> result;
	map<string, vector<Sound>>::iterator it = soundMap.begin();
	while (it != soundMap.end()) {
		int howRhyme = rhymingSyllables(it->first, rhymesWith);
		if (numberOfSyllables == countSyllables(it->first)) {
			result[howRhyme] = it->first;
		}

		it++;
	}
	string answer = "";
	if (result.size() != 0) {
		answer = result.rbegin()->second;
	}
	return answer;
}
string RhymeBot::findBestRhyme(PART_OF_SPEECH p, string rhymesWith) {
	// TODO
	set<string> temp = part_of_speech_map.at(p);
	set<string>::iterator it = temp.begin();
	int max = -1;
	string max_string = "";
	while (it != temp.end()) {
		if (rhymingSyllables(*it, rhymesWith) > max) {
			max_string = *it;
			max = rhymingSyllables(*it, rhymesWith);
		}
		it++;
	}
	return max_string;
}
string RhymeBot::findBestRhyme(int numberOfSyllables, PART_OF_SPEECH p, string rhymesWith) {
	// TODO
	set<string> temp = part_of_speech_map.at(p);
	set<string>::iterator it = temp.begin();
	int max = -1;
	string max_string = "";
	while (it != temp.end()) {
		if (numberOfSyllables == countSyllables(*it) && rhymingSyllables(*it, rhymesWith) > max) {
			max_string = *it;
			max = rhymingSyllables(*it, rhymesWith);
		}
		it++;
	}
	return max_string;
}

// returns the best word with the specified number of syllables and part of speech that forms an alliteration with
// the given word. Best is defined by the number of maximum number of alliterating sounds.
string RhymeBot::findBestAlliteration(int numberOfSyllables, string alliterationWith) {
	// TODO	
	map<int, string> result;
	map<string, vector<Sound>>::iterator it = soundMap.begin();
	while (it != soundMap.end()) {
		int howAlliter = alliterationSounds(it->first, alliterationWith);
		if (numberOfSyllables == countSyllables(it->first)) {
			result[howAlliter] = it->first;
		}

		it++;
	}
	string answer = "";
	if (result.size() != 0) {
		answer = result.rbegin()->second;
	}
	return answer;
}
string RhymeBot::findBestAlliteration(PART_OF_SPEECH p, string alliterationWith) {
	// TODO
	set<string> temp = part_of_speech_map.at(p);
	set<string>::iterator it = temp.begin();
	int max = -1;
	string max_string = "";
	while (it != temp.end()) {
		if (alliterationSounds(*it, alliterationWith) > max) {
			max_string = *it;
			max = alliterationSounds(*it, alliterationWith);
		}
		it++;
	}
	return max_string;
}
string RhymeBot::findBestAlliteration(int numberOfSyllables, PART_OF_SPEECH p, string alliterationWith) {
	// TODO
	set<string> temp = part_of_speech_map.at(p);
	set<string>::iterator it = temp.begin();
	int max = -1;
	string max_string = "";
	while (it != temp.end()) {
		if (numberOfSyllables == countSyllables(*it) && alliterationSounds(*it, alliterationWith) > max) {
			max_string = *it;
			max = alliterationSounds(*it, alliterationWith);
		}
		it++;
	}
	return max_string;
}

// Combine findBestRhyme and findBestAlliteration. Best is defined by the sum of the number of rhyming syllables
//  and the number of alliterating sounds.
string RhymeBot::findPerfectWord(int numberOfSyllables, PART_OF_SPEECH, string rhymesWith, string alliterationWith) {
	// TODO
	return "";
}