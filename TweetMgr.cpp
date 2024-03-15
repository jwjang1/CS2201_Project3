// File name: TweetMgr.cpp
// Author: Jungwon Jang
// VUnetid: jangj1
// Email: jungwon.jang@vanderbilt.edu
// Class: CS2201
// Date: 9/30/2019
// Honor statement: I have not given or received unauthorized aid on this assignment.
// Assignment Number: 3
// Description: These methods implement the TweetMgr class


#include "TweetMgr.h"
#include <string>
#include <stdexcept>
#include <cassert>

// Default Constructor
// Create an empty collection (one with zero Tweets)
TweetMgr::TweetMgr() :  numTweets(0), tweetList(nullptr){

}


// Copy ctor
TweetMgr::TweetMgr(const TweetMgr &rhs) : numTweets(rhs.numTweets), tweetList(nullptr){
    if (rhs.tweetList != nullptr){
        tweetList = new TweetNode;
        tweetList -> tweet = rhs.tweetList -> tweet;
        tweetList -> next = nullptr;
        TweetNodePtr  newPtr = tweetList;
        for (TweetNodePtr origPtr = rhs.tweetList->next; origPtr!= nullptr; origPtr=origPtr->next){
            newPtr->next = new TweetNode;
            newPtr = newPtr -> next;
            newPtr -> tweet = origPtr -> tweet;
            newPtr -> next = nullptr;
        }
    }
}


// Assignment operator
const TweetMgr &TweetMgr::operator=(const TweetMgr &rhs){
    if (this != &rhs) {
        TweetMgr tmp(rhs);
        std::swap(numTweets, tmp.numTweets);
        std::swap(tweetList, tmp.tweetList);
    }
    return *this;
}

// Destructor
TweetMgr::~TweetMgr() {
    while (tweetList != nullptr){
        TweetNodePtr tmp = tweetList;
        tweetList = tweetList->next;
        delete tmp;
    }
    numTweets = 0;
}


// getnumTweets
// Return the total number of Tweets in the collection
size_t TweetMgr::getNumTweets() const {
    return numTweets;
}


//addTweet(const Tweet &t)
//
//Purpose: add/insert a Tweet to the list of Tweet objects
//Parameters: Tweet t - the Tweet to be added
//Returns: size_t - the index position of the inserted Tweet
//
//Behavior:
//1. In case of the list being empty: insert as the first element
//2. In case of a non-empty list: insert the Tweet in sorted
//   order by timestamp
//3. In case of already existing Tweet with same timestamp: insert new tweet
//   after (behind) the existing one
//4. Return the index of the inserted tweet, using zero-based indexing
//Note: Normally this method would be a void method and not return any value. We have it
//return the index of where the tweet was inserted so that we can verify that the tweet
//was inserted in the correct position in the list.
size_t TweetMgr::addTweet(const Tweet &t) {
    size_t i = 0;
    TweetNodePtr prev;
    TweetNodePtr cur;
    TweetNodePtr add = new TweetNode;
    add->tweet = t;
    for(prev=nullptr, cur=tweetList; cur!= nullptr && cur->tweet.getTimestamp() <= t.getTimestamp();
    prev=cur, cur=cur->next){
        i++;
    }
    if (prev == nullptr){
        tweetList = add;
    }else{
        prev -> next = add;
    }
    add ->next = cur;
    numTweets++;

    return i;
}


// getTweet(size_t index)
//
// Purpose:    returns the Tweet at the specified index in the collection,
//             throw exception if index is bad
// Parameters: size_t index - the index of the desired Tweet; using zero-based indexing
// Returns:    Tweet  - the Tweet at the specified index
//
// Behavior:
// 1. If the index is invalid, throw an std::invalid_argument exception
// 2. Otherwise, return the Tweet at the specified index
// NOTE: Normally a TweetMgr class would not support indexing to access tweets, as the user
// does not think of storing tweets by index. We have only added this method so that we
// can verify that your addTweet method works correctly. You should not call this method
// in any code, as it only exists for testing and should later be deleted.
Tweet TweetMgr::getTweet(size_t index) const {
    if (index >= numTweets) {
        throw std::invalid_argument("Invalid index.");
    }
    size_t i = 0;
    TweetNodePtr cur = tweetList;
    while (cur != nullptr && i != index) {
        cur = cur->next;
        i++;
    }
    return cur->tweet;
}


// getTweets()
//
// Purpose:    Return a string of all Tweets
// Parameters: None
// Returns:    string - containing all the Tweets in sorted order with each & every
//             Tweet followed immediately by a newline character.
//             Returns an empty string if the collection is empty
std::string TweetMgr::getTweets() const {
    std::string all;
    if (numTweets == 0) {
        return all;
    } else {
        TweetNodePtr cur = tweetList;
        while (cur != nullptr) {
            all += (cur->tweet).toString() + "\n";
            cur = cur->next;
        }
        return all;
    }
}


// getTweets(const string &sender)
//
// Purpose:    Return a string of all Tweets sent by the specified sender
// Parameters: string sender - the author of the Tweets we are supposed to return
// Returns:    string   - containing all the Tweets from the specified author, in sorted order,
//                        with each Tweet followed immediately by a newline character.
//
// Behavior:
// 1. Finds all Tweets from the specified sender (matching sender is case sensitive)
// 2. If no such Tweets are found, returns an empty string
// 3. If Tweets are found, append them on the return string in sorted order each
//    followed by a newline character.
std::string TweetMgr::getTweets(const std::string &sender) const {
    std::string twt;
    if (numTweets == 0) {
        return twt;
    } else {
        TweetNodePtr cur = tweetList;
        while (cur != nullptr) {
            if((cur->tweet).getUser() == sender) {
                twt += (cur->tweet).toString() + "\n";
            }
            cur = cur->next;
        }
    }
    return twt;
}

// getTweets(size_t ts)
//
// Purpose:    Return a string of all Tweets for a given timestamp
// Parameters: size_t ts - the timestamp we are supposed to check for match
// Return:     string - containing all the matching Tweets, in sorted order,
//             each tweet followed immediately by a newline character.
//
// Behavior:
// 1. Finds all Tweeets with the given timestamp
// 2. If no match is found, returns an empty string
// 3. If matches are found, append them on the return string in sorted order each
//    followed by a newline character.
//
// Note: see addTweet() for the definition of sorted order of Tweets with the same timestamp.
std::string TweetMgr::getTweets(size_t ts) const {
    std::string timeTwt;
    timeTwt = getTweets(ts, ts);
    return timeTwt;
}


// getTweets(size_t ts1, size_t ts2)
//
// Purpose:    Return a string of all Tweets in a range of two given timestamps
// Parameters: ts1 and ts2 (the range of timestamps)
// Return:     string containing all the matching Tweets, in sorted order,
//             each Tweet followed immediately by a newline character.
//
// Behavior:
// 1. If the collection contains no timestamps in the given range, return an empty string
// 2. Create a string containing all the Tweets, in sorted order, which are later
//    than or equal to the smaller of the two timestamps and are earlier than or equal to
//    the larger of the two timestamps, with each tweet followed immediately by a
//    newline character.
// Note: the relative order of ts1 and ts2 is unknown
std::string TweetMgr::getTweets(size_t ts1, size_t ts2) const {
    std::string rangeTwt;
    if (ts1 > ts2) {
        std::swap(ts1, ts2);
    }

    if (numTweets == 0) {
        return rangeTwt;
    }
    else if (ts1 == ts2){ //one ts
        TweetNodePtr cur = tweetList;
        while (cur != nullptr) {
            if (cur->tweet.getTimestamp() == ts1) {
                rangeTwt += (cur->tweet).toString() + "\n";
            }
            cur = cur->next;
        }
    }
    else {
        TweetNodePtr cur = tweetList;
        while (cur != nullptr) {
            if (cur->tweet.getTimestamp() >= ts1 && cur->tweet.getTimestamp() <= ts2) {
                rangeTwt += (cur->tweet).toString() + "\n";
            }
            cur = cur->next;
        }
    }
    return rangeTwt;
}

// deleteTweets()
//
// Purpose:    Deletes all Tweets from the collection
// Parameters: None
// Return:     size_t - the number of Tweets deleted (a size_t value)
size_t TweetMgr::deleteTweets() {

    /*TweetNodePtr prev = tweetList;
    while (prev != nullptr){
        TweetNodePtr dlt = prev;
        prev = prev -> next;
        delete dlt;
    }
    delete prev;
    prev = nullptr;*/
    while (tweetList != nullptr){
        TweetNodePtr dlt = tweetList;
        tweetList = tweetList -> next;
        delete dlt;
    }

    size_t i = numTweets;
    numTweets = 0;
    return i;
}

// deleteTweets(const string &sender)
//
// Purpose:    Delete all Tweets from the specified sender
// Parameters: string sender - author of tweets to be deleted, comparison is case sensitive
// Return:     size_t - number of Tweets deleted (size_t value)
size_t TweetMgr::deleteTweets(const std::string &sender) {
    size_t senderdlt = 0;

    TweetNodePtr cur = tweetList;
    TweetNodePtr prev = tweetList;
    while (cur != nullptr) {
        if (cur->tweet.getUser() == sender) {
            if (cur == tweetList) {
                tweetList = cur->next;
                delete cur;
                cur = tweetList;
                senderdlt++;
            } else {
                prev->next = cur->next;
                delete cur;
                cur = tweetList;
                senderdlt++;
            }
        }else {
            cur = cur->next;
        }
    }
    return senderdlt;
}


// deleteTweets(size_t ts)
//
// Purpose:    Deletes all Tweets whose timestamps are earlier than the parameter ts
// Parameters: size_t ts - the specified timestamp
// Return:     size_t - the number of Tweets deleted (a size_t value)
//
// Notes:
// 1. If there are no timestamps earlier than ts, no deletions will be performed and
//    zero is returned
size_t TweetMgr::deleteTweets(size_t ts) {
    size_t timedlt = 0;
    if (ts == 0) {
        timedlt = 0;
    }
    else {
        timedlt = deleteTweets(0, ts - 1);
    }

    return timedlt;
}


// deleteTweets(size_t ts1, size_t ts2)
//
// Purpose:    Deletes all Tweets between a range of two given timestamps
// Parameters: ts1 and ts2 - the range of timestamps
// Return:     size_t - number of Tweets deleted (size_t value)
//
// Behavior:
// 1. If the collection contains no timestamps in the given range, perform no deletions
//    and return zero
// 2. Delete all Tweets which are later than or equal to the smaller of the two
//    timestamps and are earlier than or equal to the larger of the two timestamps.
//    Return the number deleted.
// Note: the relative order of ts1 and ts2 is unknown
size_t TweetMgr::deleteTweets(size_t ts1, size_t ts2) {
    size_t rangedlt=0;

    if (ts1 > ts2) {
        std::swap(ts1, ts2);
    }

    if (numTweets == 0) {
        return rangedlt;
    }
    else if (ts1 == ts2){
        TweetNodePtr cur = tweetList;
        TweetNodePtr prev = tweetList;
        while (cur != nullptr) {
            if (cur->tweet.getTimestamp() == ts1) {
                if(cur == tweetList){
                    tweetList = cur->next;
                    delete cur;
                    cur = tweetList;
                    rangedlt++;
                }else{
                    prev->next = cur->next;
                    delete cur;
                    cur = tweetList;
                    rangedlt++;
                }
            }else {
                cur = cur->next;
            }
        }
    }

    else {
        TweetNodePtr cur = tweetList;
        TweetNodePtr prev = tweetList;
        while (cur != nullptr) {
            if (cur->tweet.getTimestamp() >= ts1 && cur->tweet.getTimestamp() <= ts2) {
                if(cur == tweetList){
                    tweetList = cur->next;
                    delete cur;
                    cur = tweetList;
                    rangedlt++;
                }else{
                    prev->next = cur->next;
                    delete cur;
                    cur = tweetList;
                    rangedlt++;
                }
            }
            else {
                cur = cur->next;
            }
            prev = cur;
        }
    }

    return rangedlt;
}


// merge(const TweetMgr& tm)
//
// Purpose:    Merge a received tweet collection into this collection
// Parameters: TweetMgr tm (the TweetMgr collection to be merged)
// Return:     None
//
// Behavior:
// 1. Add each tweet of the parameter TweetMgr to the object TweetMgr;
//    may result in duplicate entries if both TweetMgrs had the same tweets.
// 2. All tweets that are added remain in the same order as they appeared in the
//    parameter TweetMgr.
// 3. Any added tweet with the same timestamp of an existing tweet is added
//    after (behind) the existing tweet.
// 4. You are guaranteed that the parameter TweetMgr object tm is a different
//    TweetMgr object than the ‘this’ TweetMgr object.
void TweetMgr::merge(const TweetMgr& tm) {
    TweetNodePtr add = tm.tweetList;
    while(add != nullptr){
        addTweet(add->tweet);
        add = add->next;
    }
}