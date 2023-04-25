/*
* FILE: main.cpp
* AUTHOR: Morgan Purcell
* DATE: 04/2023
* PURPOSE: main function for finding the maximum sum sequence
*/


// Include the necessary libraries
#include <iostream>
#include <fstream>
#include <vector>


// A function to fidn the optimal score for each inning and find the total max sum
void findAnswer(std::vector<char>& scores, std::vector<std::vector<long int> >& scoreboard)
{
    // Variable to hold the total score for all 9 innings combines
    long int total = 0;
    // Vector of strings to hold the score for each inning
    std::vector<std::string> answer;
    // Bool to determine if the score has been found for an inning
    bool found_score = false;

    // Start at row 9
    int row = 9;
    // Iterate over each column starting at the last column
    for (int col = scores.size()-1; col > 0; col--)
    {   
        // String to hold the score for an inning
        std::string str_inning_answer = "";
        // Reset the bool to indicate if an innings score has been found
        found_score = false;

        // Iterate back by 1 row and k columns to find the optimal score for an index
        for (int k = col-1; k >= 0; k--)
        {
            // Add tthe current indexes score/char to tje string answer
            str_inning_answer.insert(0,1,scores[k+1]);
            // If the current index we're looking at - the long int version of the string answer = the index at row-1 and back k columns
            // the answer has been found
            if (scoreboard[row][col] - std::stol(str_inning_answer) == scoreboard[row-1][k])
            {
                // Update the total with the long int inning answer char(s)
                total += std::stol(str_inning_answer);
                // Add the string inning answer to the answer vector
                answer.push_back(str_inning_answer);
                // The previous score has been found
                found_score = true;
            }
            if(found_score)
            {
                // Start searching from column k+1 next iteration
                col = k+1;
                // Move to the previous row
                row -= 1;
                // Break out of this loop to move to the next index to search for
                break;
            }
        }
    }
    
    std::cout << "\n| ";

    for (int i = answer.size()-1; i >= 0; i--)
    {
        std::cout << answer[i] << " | ";
    }
    std::cout << "\nTotal: " << total << "\n\n"; 
    

    return;
}


// A function to find the max sum sequence that takes a vector containing the scores/chars input and a 2D vector to hold the optimal scores as arguments
void findMaxSumSeq(std::vector<char>& scores, std::vector<std::vector<long int> >& scoreboard)
{
    // Base case - inning 0 and score/char 0 hold an optimal score of 0
    scoreboard[0][0] = 0;

    // variable to hold a negative long int
    const long int large_neg_num = LONG_MIN;

    // Set the inital values in scoreboard
    for (int row = 0; row < 10; row++)
    {
        for (int col = 0; col < scores.size(); col++)
        {
            // An index with available innings and no available scores/chars is impossible so set it to a large negative long int
            if (row>=1 && col==0)
                scoreboard[row][col] = large_neg_num;
            // An index with available scores/chars and no available innings is impossible so set it to a large negative long int
            else if (row==0 && col>=1)
                scoreboard[row][col] = large_neg_num;
            // An index with more available innings than there is scores/chars is impossible so set it to a large negative long int
            else if (row > col)
                scoreboard[row][col] = large_neg_num;
        }
    }

    // Iterate through the 2D array and fill in the optimal scores at each valid index
    for (int row = 1; row < 10; row++)
    {
        for (int col = 1; col < scores.size(); col++)
        {
            // Skip the invalid "-" indexes
            if (scoreboard[row][col] != large_neg_num)
            {
                // Set the index being solving for, to the previous indexes score for now
                scoreboard[row][col] = scoreboard[row-1][col];
                // Initialze the score to be set to the index being solved for - 0 for now
                long int score = 0;
                // Store the previous inning/row number
                int pre_inning = row-1;
                // Initialize an empty string to hold the string score
                std::string str_score = "";
                // Variable to to make sure we only backtrack at most 17 times to build up the score
                int backtrack_cap = 0;
                // Iterate back by 1 row and k columns to find the optimal score for an index
                for (int k = col-1; k >= 0 && backtrack_cap < 17; k--)
                {
                    // Increment the backtrack cap
                    backtrack_cap ++;
                    // Insert the current col+1's char value into the str_score
                    str_score.insert(0,1,scores[k+1]);
                    // Initalize a long int new score variable to the string representation of the current string score + the 2D array index being looked at
                    long int newScore = std::stol(str_score) + scoreboard[pre_inning][k];
                    // If the new score beats the current score
                    if (newScore > score)
                    {
                        // Update the score and 2D array index being solved for
                        score = newScore;
                        scoreboard[row][col] = newScore; 
                    }
                }
            }   
        }
    }
    
    // Print out the 2D vector
    // for (int i = 0; i < 10; i++)
    // {
    //     for (int j = 0; j < scores.size(); j++)
    //     {
    //         std::cout << scoreboard[i][j] << "\t";
    //     }
    //     std::cout << "\n\n\n";
    // }

    // Call the function to print the innings, scores, and total
    findAnswer(scores, scoreboard);


    return;
}


int main(int argc, char **argv)
{
    // Vector to hold the available scores/chars
    std::vector<char> scores;
    // Start the scores at index 1
    scores.push_back('0');

    // Pull the scores from the command line
    std::string input = argv[1];
    // Fill the scores vector with the scores/chars given on the command line 
    for (char c : input)
        scores.push_back(c);

    // Check if their is sufficientt input
    if (scores.size() < 10 || scores.size() > 154)
    {
        std::cerr << "Invalid data entry";
        return 1;
    }

    // 2D vector to hold the optimal scores with innings as rows and scores/chars as columns
    std::vector<std::vector<long int> > scoreboard(10, std::vector<long int>(scores.size()));

    // Call the function to find the max sum sequence
    findMaxSumSeq(scores, scoreboard);
    

    return 0;
}
