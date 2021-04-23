/**
 * Subject: Bachelor's thesis
 * Author: Adam Sedlacek | xsedla1e@vutbr.cz
 * Year: 2021
 * Description:
 *      State enumerate - literal's virtual states.
 *      Not   - negation
 *      Is    - active input
 *      IsNot - inactive element/input
 * 
 */

#ifndef STATE_H
#define STATE_H

enum class State {
    Not,
    Is,
    IsNot
};

#endif /* STATE_H */