#ifndef EVALUATION_H
#define EVALUATION_H

#include <vector>
# include "structures.h"
#include "submission_file.h"
#include <string>

using namespace std;



class Evaluation {
public:
    double evaluation_score(const std::vector<SubmissionRow>& submission); 
    double parseValue(const std::string& value);
    std::string extractGroup(const std::string& id);
    std::vector<SubmissionRow> loadSubmissionFile(const std::string filename);
};


#endif

