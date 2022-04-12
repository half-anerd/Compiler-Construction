#include "typeChecker.h"

void semanticAnalyser(ast* root){
    //populate symbol tables
    ast * curr = root->firstChild;

    if(curr==NULL)
        return;

    while(curr->nodeType == FUNCTION_SEQ){
        validateFunction(curr);
        curr = curr->nextSibling;
    }
    validateMain(curr);
}

void validateFunction(ast* curr){
    if(curr == NULL)
        return;

    functionNode* currNode = (functionNode*)malloc(sizeof(functionNode));
    functionNode* discardNode = currNode;
    currNode = (functionNode*)retrieve(); //update according to retrieve
    free(discardNode);
    int n = currNode->numOp;
    bool* opAssigned = (bool*)malloc(sizeof(bool)*n);

    // //check for otherstmts
    // ast* currNext = curr->firstChild->nextSibling->nextSibling->nextSibling;
    // if(currNext->nodeType == OTHERSTMTS){
    //     validateStmts(currNext, currNode, opAssigned);
    // }
    // else return;


    for(int i = 0; i < n; i++){
        if(opAssigned[i] == true)
            continue;
        else{
            //semantic error -- op parameter not assigned, change flag
            //print semantic error to file
        }
    }
    free(opAssigned);

    ast* ip = curr->firstChild;
    if(ip->nodeType == INPUT_PARAMETERS)
        validateIP(ip);
    
    ast* nextKid = curr->firstChild->nextSibling;
    ast* child = NULL;
    if(nextKid->nodeType == OUTPUT_PARAMETERS) {
        ast* op = nextKid;
        validateOP(op);
    }
    else {
        child = nextKid;
    }

    if(child == NULL) {
        child = curr->firstChild->nextSibling->nextSibling;
    }

    for(; child != NULL; child = child->nextSibling) {
        int localOffset = 0;
        NodeType nType = child->nodeType;
        //validateStmts(ast* curr, functionNode* currNode, bool* opAssigned);

    }
}

// void validateStmts(ast* curr, functionNode* currNode, bool* opAssigned){

// }
bool isOperator(ast* curr) {
    NodeType nType = curr->nodeType;
    if(nType == DIVIDE || nType == MULTIPLY || nType == PLUS || nType == MINUS) {
        return true;
    }
    else {
        return false;
    }
}

bool isIntReal(ast* curr) {
    NodeType nType = curr->nodeType;
    if(nType == INTEGER || nType == REAL) {
        return true;
    }
    else {
        return false;
    }
}

// bool isRecUnion(identifierNode* currNode) {
//     Type nType = currNode->type;
//     if(nType == RECORD_TYPE || nType == UNION_TYPE) {
//         return true;
//     }
//     else {
//         return false;
//     }
// }

identifierNode* validateArithmetic(ast* curr, ast* func) {
    if (curr == NULL)
    {
        return;
    }
    int localOffset = 0;
    //int numTempVar = 0;

    //identifier node
    if(isOperator(curr) == false){
        identifierNode* currNode = createINode(curr, func, curr->nodeType, false, &localOffset);
        identifierNode* notGlobalCurr = currNode;
        notGlobalCurr->global = false;
        notGlobalCurr = retrieve(notGlobalCurr);
        identifierNode* globalCurr = currNode;
        globalCurr->global = true;
        globalCurr = retrieve(globalCurr);
        if (globalCurr)
        {
            currNode = globalCurr;
        }
        else if (notGlobalCurr){
            currNode = notGlobalCurr;
        }
        else {
            free(notGlobalCurr);
            free(globalCurr);
            //semanticErrors ++;
            printf("Undeclred Variable\n");
            return NULL;
        }
        
        free(notGlobalCurr);
        free(globalCurr);
        
        ast* attribute = NULL;
        if(!curr->firstChild) {
            attribute = curr->firstChild;
        }
        if(currNode->type != RECORD_TYPE && attribute != NULL){
            printf("Variable not of type record.\n");
            //semanticErrors++;
        }
        else if (currNode)

    }

    //operator node
    else if(isOperator(curr) == true) {

        ast* fChild, *sChild;
        fChild = curr->firstChild;
        sChild = fChild->nextSibling;
        //ast has 1 or no children
        if(fChild==NULL || sChild == NULL) {
            return;
        }

        identifierNode *fNode, *sNode;
        fNode = retrieve(fChild);
        sNode = retrieve(sChild);
        //node(s) not found in symbol table
        if(fNode==NULL || sNode == NULL) {
            return;
        }
        Type fType = fNode->type;
        Type sType = sNode->type;

        if(isOperator(fChild) == true){
            fNode = validateArithmetic(fChild, func);
            fType = fNode->type;
        }
        else if (isOperator(sChild) == true){
            sNode = validateArithmetic(sChild, func);
            sType = sNode->type;
        }

        //both are identifiers
        else if(isOperator(fChild) == false && isOperator(sChild) == false) {

            //both int/real
            if(isIntReal(fChild) == true && isIntReal(sChild) == true){
                //both int, both real;
                if(fType == sType) {
                    return createINode(fChild, func, fChild->nodeType, false, &localOffset);
                }
                //int with real operations  
                else {
                    return NULL;
                }
            }

            //both records/unions
            else if(fChild->nodeType == RECORD_OR_UNION && sChild->nodeType == RECORD_OR_UNION) {
                if(fType == sType){
                    //record
                    if(fType == RECORD_TYPE && fNode->recordList == sNode->recordList) {
                        return fNode;
                    }
                    //union
                    if(fType == UNION_TYPE && fNode->recordList == sNode->recordList){
                        return fNode;
                    }
                }
                //operations bw rec and union
                else {
                    return NULL;
                }
            }

            //record/union and int/real
            else {
                if(curr->nodeType != DIVIDE){
                    if(fType == RECORD_TYPE) {
                        return fNode;
                    }
                    else if (fType == UNION_TYPE) {
                        return fNode;
                    }
                    else if(sType == RECORD_TYPE) {
                        return sNode;
                    }
                    else if (sType == UNION_TYPE) {
                        return sNode;
                    }
                }
                else{
                    if(sChild->nodeType == RECORD_OR_UNION){
                        printf("Can't divide scalar by record/union");
                        //semantic_errors++;
                        return NULL;
                    }
                    else{
                        if(fType == RECORD_TYPE) {
                            return fNode;
                        }
                        else if (fType == UNION_TYPE) {
                            return fNode;
                    }
                }
            }   
        }

        // end of both identifiers
    }

    }

    else {
        return NULL;
    }
}