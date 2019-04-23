#pragma once

#include <iostream>
#include <string>

#define BLACK 'X'
#define WHITE 'O'
#define SIZE 5

class Room
{
private:
    uint32_t one;
    uint32_t two;
    char piece[2];
    uint32_t current;
    char board[SIZE][SIZE];
    char result; // X, O, E, N

    pthread_mutex_t lock;

public:
    Room()
    {
    }

    Room(uint32_t &id1, uint32_t &id2):one(id1),two(id2)
    {
        piece[0] = 'X';
        piece[1] = 'O';
        memset(board, ' ', sizeof(board));
        result = 'N';
        pthread_mutex_init(&lock, NULL);
    }

    void Board(string &_board)
    {
        for(auto i = 0; i < SIZE; i++)
        {
            for(auto j = 0; j < SIZE; i++)
            {
                _board.push_back(board[i][j]);
            }
        }
    }

    char Piece(uint32_t &id)
    {
        int pos = (id == one ? 0 : 1);
        return piece[pos];
    }

    ~Room()
    {
        pthread_mutex_destroy(&lock);
    }
};
