
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


typedef enum
{
    ID_TOKEN,
    PROGRAM_TOKEN,
    CONST_TOKEN,
    VAR_TOKEN,
    BEGIN_TOKEN,
    END_TOKEN,
    IF_TOKEN,
    THEN_TOKEN,
    WHILE_TOKEN,
    DO_TOKEN,
    READ_TOKEN,
    WRITE_TOKEN,
    PV_TOKEN,
    PT_TOKEN,
    PLUS_TOKEN,
    MOINS_TOKEN,
    MULT_TOKEN,
    DIV_TOKEN,
    VIR_TOKEN,
    AFF_TOKEN,
    INF_TOKEN,
    INFEG_TOKEN,
    SUP_TOKEN,
    SUPEG_TOKEN,
    DIFF_TOKEN,
    PO_TOKEN,
    PF_TOKEN,
    FIN_TOKEN,
    NUM_TOKEN,
    ERREUR_TOKEN,
    EOF_TOKEN,
    EG_TOKEN,
    STRING_TOKEN ,
    REAL_TOKEN , 
   
    BOOLEAN_TOKEN,
    Tinteger_TOKEN ,
    Tstring_TOKEN ,
    Treal_TOKEN ,
    Tboolean_TOKEN ,
    Tchar_TOKEN,
    ARRAY_TOKEN ,
    CO_TOKEN ,
    CF_TOKEN ,//{
    OF_TOKEN ,//}
    ELSE_TOKEN , 
    FOR_TOKEN ,
    TO_TOKEN ,
    DOWNTO_TOKEN ,
    CASE_TOKEN , 
    DPT_TOKEN ,
    TRUE_TOKEN ,
    FALSE_TOKEN ,
    APT_TOKEN,
    TYPE_TOKEN,
} CODES_LEX;

// erreur types
typedef enum
{
    ID_ERR,
    PROGRAM_ERR,
    CONST_ERR,
    VAR_ERR,
    BEGIN_ERR,
    END_ERR,
    IF_ERR,
    THEN_ERR,
    WHILE_ERR,
    DO_ERR,
    READ_ERR,
    WRITE_ERR,
    PV_ERR,
    PT_ERR,
    PLUS_ERR,
    MOINS_ERR,
    MULT_ERR,
    DIV_ERR,
    VIR_ERR,
    AFF_ERR,
    INF_ERR,
    INFEG_ERR,
    SUP_ERR,
    SUPEG_ERR,
    DIFF_ERR,
    PO_ERR,
    PF_ERR,
    FIN_ERR,
    NUM_ERR,
    ERREUR_ERR,
    EOF_ERR,
    EG_ERR,
    REAL_ERR,
    STRING_ERR,
    CHAR_ERR ,
    BOOLEAN_ERR,
    Tinteger_ERR ,
    Tstring_ERR ,
    Treal_ERR ,
    Tboolean_ERR ,
    Tchar_ERR ,
    ARRAY_ERR ,
    CO_ERR ,
    CF_ERR ,//{
    OF_ERR ,//}
    ELSE_ERR , 
    FOR_ERR ,
    TO_ERR ,
    DOWNTO_ERR ,
    CASE_ERR , 
    DPT_ERR ,
    TRUE_ERR ,
    FALSE_ERR ,
    APT_ERR ,
    IDLIST_ERR,
    TYPE_ERR, 
    //Sémantique
    IDENTIFIER_ALREADY_DECLARED_ERR,
    UNDECLARED_IDENTIFIER_ERR,
    CONST_VAR_BEGIN_ERR,
    CONST_MODIFICATION_ERR,
    PROGRAM_TYPE_AFTER_BEGIN_ERR,
    VAR_BEGIN_ERR,
    INST_PCODE_ERR
} CODES_ERR;

typedef struct
{
    CODES_LEX CODE;
    char NOM[20];
    int val;
} TSym_Cour;

TSym_Cour SYM_COUR;

typedef enum 
{   TPROG,
    TCONST,
    TVAR,
    Ttab
} TSYM;

typedef struct {
    char NOM[20];
    TSYM TIDF; // TSYM should be an enumeration indicating whether it's a constant or variable
   
} T_TAB_IDF;

#define MAX_IDFS 100 // Set an appropriate maximum number of identifiers
T_TAB_IDF TAB_IDFS[MAX_IDFS]; 
int NbrIDFS = 0; // Variable to keep track of the number of identifiers

FILE *fichier;

char Car_Cour; // caractère courant

//=========================================================================================================//
//=========================================================================================================//
//gerateur de code//

typedef struct {
    char NOM[20];
    int valeur;  
    int ADRESSE;
} ENREGISTREMENT;

// Nouvelle déclaration de la table des symboles
#define TABLEINDEX 100
ENREGISTREMENT TABLESYM[TABLEINDEX];

// Variables globales
int OFFSET;  // Remplace la définition précédente de OFFSET
int SP;      // SP: sommet de la pile


typedef enum {
    ADD, SUB, MUL, DIV, EQL, NEQ, GTR, LSS, GEQ, LEQ, PRN,
    INN, INT, LDI, LDA, LDV, STO, BRN, BZE, HLT
} MNEMONIQUES;

MNEMONIQUES op;

typedef struct {
    MNEMONIQUES MNE;
    int SUITE;
} INSTRUCTION;

#define TAILLECODE 100
INSTRUCTION PCODE[TAILLECODE];  // Tableau représentant les instructions de P-Code
int PC = 0;                     // Compteur d'instruction

int OFFSET = -1;  // OFFSET initialisé à -1
int IND_DER_SYM_ACC = 0;

//
void GENERER1(MNEMONIQUES M) {
    if (PC == TAILLECODE) {
        // Gérer l'erreur
        printf("Erreur : PC atteint la taille maximale.\n");
        exit(EXIT_FAILURE);
    }

    PC++;
    PCODE[PC].MNE = M;
}

void GENERER2(MNEMONIQUES M, int A) {
    if (PC == TAILLECODE) {
        // Gérer l'erreur
        printf("Erreur : PC atteint la taille maximale.\n");
        exit(EXIT_FAILURE);
    }

    PC++;
    PCODE[PC].MNE = M;
    PCODE[PC].SUITE = A;
}


//=========================================================================================================//
//=========================================================================================================//

// functions declaration
void VARS();
void INSTS();
void INST();
void AFFEC();
void SI();
void TANTQUE();
void ECRIRE();
void LIRE();
void EXPR();
void TERM();
void FACT();
void MULOP();
void ADDOP();
void RELOP();
void COND();
void Lire_Car();
void Erreur(CODES_ERR code);
void Test_Symbole(CODES_LEX cl, CODES_ERR COD_ERR);
void PROGRAM();
void BLOCK();
void CONSTS();
void TABS();
void CONSTVAL();
void IDLIST();
void TYPE();
void ARRAY();
void ELEMENT();
void IF();
void FOR();
void CASE();
void CASE_BRANCH();
void VALUE();
void ARRAY_ACCESS();
void BOOL();
void STRING();
void Sym_Suiv();
void lire_mot();
void lire_nombre();
int getIdentifierIndex(char *identifier);
void lire_chaine()
{   
    char chaine[20];
    int indice = 0;

    Lire_Car(); // Skip the opening single quote

    while (Car_Cour != EOF && Car_Cour != '\'')
    {
        chaine[indice++] = Car_Cour;
        Lire_Car();
    }

    // Check if the string is properly terminated
    if (Car_Cour == '\'')
    {
        Lire_Car(); // Skip the closing single quote
        chaine[indice] = '\0';

        // Store the string in the token
        SYM_COUR.CODE = STRING_TOKEN;
        strcpy(SYM_COUR.NOM, chaine);
    }
    else
    {
        // Unterminated string, report an error
        Erreur(ERREUR_ERR);
    }
}
// functions definition
int isIdentifierDeclared(char *identifier) {
    for (int i = 0; i < NbrIDFS; i++) {
        if (strcmp(TAB_IDFS[i].NOM, identifier) == 0) {
            return 1; // Identifier is already declared
        }
    }
    return 0; // Identifier is not declared
}

void addIdentifier(char *identifier, TSYM type) {
    // Check for double declarations
    if (isIdentifierDeclared(identifier)) {
        Erreur(IDENTIFIER_ALREADY_DECLARED_ERR);
    }
    // Add the identifier to the symbol table
    strcpy(TAB_IDFS[NbrIDFS].NOM, identifier);
    TAB_IDFS[NbrIDFS].TIDF = type;
    NbrIDFS++;
}
int isIdentifierUsed(char *identifier) {
    // Parcourir la table des identifiants déjà utilisés
    for (int i = 0; i < NbrIDFS; ++i) {
        // Vérifier si l'identifiant actuel correspond à un identifiant utilisé
        if (strcmp(TAB_IDFS[i].NOM, identifier) == 0) {
            return 1; // Identifiant déjà utilisé
        }
    }
    return 0; // Identifiant non utilisé
}

void semanticCheckAfterBegin() {
    for (int i = 0; i < NbrIDFS; i++) {
        if (!isIdentifierUsed(TAB_IDFS[i].NOM)) {
            // Identifier used after BEGIN must be declared
            Erreur(UNDECLARED_IDENTIFIER_ERR);
        }
        int idIndex = getIdentifierIndex(SYM_COUR.NOM);
        if (idIndex != -1 && TAB_IDFS[idIndex].TIDF == TPROG) {
            // Le type TPROG ne peut pas être utilisé après BEGIN
            Erreur(PROGRAM_TYPE_AFTER_BEGIN_ERR);
        }
        
    }
}



void lire_mot()
{
    char mot[20];
    int indice = 0;
    // Lecture du premier caractère (lettre)
    mot[indice++] = Car_Cour;
    Lire_Car();

    // Lecture des caractères suivants (lettres ou chiffres)
    while (isalpha(Car_Cour) || isdigit(Car_Cour))
    {
        mot[indice++] = Car_Cour;
        Lire_Car(); //jusqu'au terminer le mot
    }

    // Ajout du caractère de fin de chaîne
    mot[indice] = '\0'; 

    // Vérifier si le mot est un mot-clé
    if (strcasecmp(mot, "program") == 0)
    {
        SYM_COUR.CODE = PROGRAM_TOKEN;
    }
    else if (strcasecmp(mot, "const") == 0)
    {
        SYM_COUR.CODE = CONST_TOKEN;
    }
    else if (strcasecmp(mot, "var") == 0)
    {
        SYM_COUR.CODE = VAR_TOKEN;
    }
    else if (strcasecmp(mot, "type") == 0)
    {
        SYM_COUR.CODE = TYPE_TOKEN;
    }
    else if (strcasecmp(mot, "begin") == 0)
    {
        SYM_COUR.CODE = BEGIN_TOKEN;
    }
    else if (strcasecmp(mot, "end") == 0)
    {
        SYM_COUR.CODE = END_TOKEN;
    }
    else if (strcasecmp(mot, "if") == 0)
    {
        SYM_COUR.CODE = IF_TOKEN;
    }
    else if (strcasecmp(mot, "then") == 0)
    {
        SYM_COUR.CODE = THEN_TOKEN;
    }
    else if (strcasecmp(mot, "while") == 0)
    {
        SYM_COUR.CODE = WHILE_TOKEN;
    }
    else if (strcasecmp(mot, "do") == 0)
    {
        SYM_COUR.CODE = DO_TOKEN;
    }
    else if (strcasecmp(mot, "read") == 0)
    {
        SYM_COUR.CODE = READ_TOKEN;
    }
    else if (strcasecmp(mot, "write") == 0)
    {
        SYM_COUR.CODE = WRITE_TOKEN;
    }
    else if (strcasecmp(mot, "Integer") == 0)
    {
        SYM_COUR.CODE = Tinteger_TOKEN ;
    }
    else if (strcasecmp(mot, "String") == 0)
    {
        SYM_COUR.CODE = Tstring_TOKEN ;
    }
    else if (strcasecmp(mot, "Real") == 0)
    {
        SYM_COUR.CODE = Treal_TOKEN ;
    }
    else if (strcasecmp(mot, "Boolean") == 0)
    {
        SYM_COUR.CODE = Tboolean_TOKEN ;
    }
    else if (strcasecmp(mot, "Char") == 0)
    {
        SYM_COUR.CODE = Tchar_TOKEN ;
    }

    else if (strcasecmp(mot, "array") == 0)
    {
        SYM_COUR.CODE = ARRAY_TOKEN;
    }
    else if (strcasecmp(mot, "of") == 0)
    {
        SYM_COUR.CODE = OF_TOKEN ;
    }
    else if (strcasecmp(mot, "else") == 0)
    {
        SYM_COUR.CODE = ELSE_TOKEN ;
    }
    else if (strcasecmp(mot, "for") == 0)
    {
        SYM_COUR.CODE = FOR_TOKEN ;
    }
    else if (strcasecmp(mot, "to") == 0)
    {
        SYM_COUR.CODE = TO_TOKEN ;
    }
    else if (strcasecmp(mot, "downto") == 0)
    {
        SYM_COUR.CODE = DOWNTO_TOKEN ;
    }
    else if (strcasecmp(mot, "case") == 0)
    {
        SYM_COUR.CODE = CASE_TOKEN ;
    }
    else if (strcasecmp(mot, "true") == 0)
    {
        SYM_COUR.CODE = TRUE_TOKEN ;
    }
    else if (strcasecmp(mot, "false") == 0)
    {
        SYM_COUR.CODE = FALSE_TOKEN ;
    }
    else
    {
        // If it's not a keyword, it's an identifier
        SYM_COUR.CODE = ID_TOKEN;
    }

    // Stockage du mot dans le jeton
    strcpy(SYM_COUR.NOM, mot);
}

void lire_nombre()
{
    char nombre[20];
    int indice = 0;

    // Lecture du premier chiffre
    nombre[indice++] = Car_Cour;
    Lire_Car();

    // Lecture des chiffres suivants
    while (isdigit(Car_Cour))
    {
        nombre[indice++] = Car_Cour;
        Lire_Car();
    }

 // Si un point est rencontré, c'est un nombre décimal
    if (Car_Cour == '.')
    {
        nombre[indice++] = Car_Cour;
        Lire_Car();

        // Lecture des chiffres décimaux
        while (isdigit(Car_Cour))
        {
            nombre[indice++] = Car_Cour;
            Lire_Car();
        }

        // Ajout du caractère de fin de chaîne
        nombre[indice] = '\0';

        // Stockage du nombre dans le jeton
        SYM_COUR.CODE = REAL_TOKEN  ;
       
        SYM_COUR.val = atof(nombre); // Conversion vers un nombre flottant
    }
    else
    {
        // Ajout du caractère de fin de chaîne
        nombre[indice] = '\0';

        // Stockage du nombre dans le jeton
        SYM_COUR.CODE = NUM_TOKEN;
      
        SYM_COUR.val = atoi(nombre); // Conversion vers un nombre entier
    }
}

void Sym_Suiv()
{
    while (Car_Cour == ' ' || Car_Cour == '\n' || Car_Cour == '\t')
    {
        Lire_Car();
    }
    if (isalpha(Car_Cour))
    {
        lire_mot();
    }
    else if (isdigit(Car_Cour))
    {
        lire_nombre();
    }else if (Car_Cour == '\'')
    {
         lire_chaine();
    }
    else
    {
        switch (Car_Cour)
        {
        case ';':
            SYM_COUR.CODE = PV_TOKEN;
            Lire_Car();
            break;

        case '+':
            SYM_COUR.CODE = PLUS_TOKEN;
            Lire_Car();
            break;

        case '\'':
            SYM_COUR.CODE = APT_TOKEN;
            Lire_Car();
            break;

        case '-':
            SYM_COUR.CODE = MOINS_TOKEN;
            Lire_Car();
            break;

        case '*':
            SYM_COUR.CODE = MULT_TOKEN;
            Lire_Car();
            break;

        case '/':
            SYM_COUR.CODE = DIV_TOKEN;
            Lire_Car();
            break;

        case ',':
            SYM_COUR.CODE = VIR_TOKEN;
            Lire_Car();
            break;
        case '[':
            SYM_COUR.CODE = CO_TOKEN ;
            Lire_Car();
            break;
        case ']':
            SYM_COUR.CODE = CF_TOKEN ;
            Lire_Car();
            break;

        case ':':
            Lire_Car();
            if (Car_Cour == '=')
            {
                SYM_COUR.CODE = AFF_TOKEN;
                Lire_Car();
            }
            else
            {
                SYM_COUR.CODE = DPT_TOKEN ;
            }
            break;

        case '<':
            Lire_Car();
            if (Car_Cour == '=')
            {
                SYM_COUR.CODE = INFEG_TOKEN;
                Lire_Car();
            }
            else if (Car_Cour == '>')
            {
                SYM_COUR.CODE = DIFF_TOKEN;
                Lire_Car();
            }
            else
            {
                SYM_COUR.CODE = INF_TOKEN;
            }
            break;

        case '>':
            Lire_Car();
            if (Car_Cour == '=')
            {
                SYM_COUR.CODE = SUPEG_TOKEN;
                Lire_Car();
            }
            else
            {
                SYM_COUR.CODE = SUP_TOKEN;
            }
            break;

        case '(':
            SYM_COUR.CODE = PO_TOKEN;
            Lire_Car();
            break;
        case '=':
            SYM_COUR.CODE = EG_TOKEN;
            Lire_Car();
            break;

        case ')':
            SYM_COUR.CODE = PF_TOKEN;
            Lire_Car();
            break;

        case '.':
            
            SYM_COUR.CODE = PT_TOKEN;
            break;

        case EOF:
            SYM_COUR.CODE = FIN_TOKEN;
            break;

        default:
            SYM_COUR.CODE = ERREUR_TOKEN;
            Lire_Car();
        }
    }
}

void Lire_Car()
{
    Car_Cour = fgetc(fichier);
}

void Erreur(CODES_ERR code)
{
    printf("Erreur: %d\n", code);
    switch (code)
    {
    case IDENTIFIER_ALREADY_DECLARED_ERR:
        printf("Erreur: L'identifiant '%s' est déjà déclaré.\n", SYM_COUR.NOM);
        break;
    case UNDECLARED_IDENTIFIER_ERR:
        printf("Erreur: L'identifiant '%s' n'est pas déclaré.\n", SYM_COUR.NOM);
        break;
    case CONST_VAR_BEGIN_ERR:
        printf("Erreur: Les constantes doivent être déclarées avant les variables et le programme principal.\n");
        break;
    case CONST_MODIFICATION_ERR:
        printf("Erreur: Tentative de modification d'une constante.\n");
        break;
    case VAR_BEGIN_ERR:
        printf("Erreur: Les variables doivent être déclarées avant le début des instructions.\n");
        break;
    case PROGRAM_TYPE_AFTER_BEGIN_ERR:
        printf("Erreur: L'identifiant '%s' non autorisé car c'est nom de programme  \n", SYM_COUR.NOM);
        break;
    // ...
    default:
        printf("Erreur inconnue.\n");
        break;
    }
    printf("Current Token: %d\n", SYM_COUR.CODE);
    printf("Current Lexeme: %s\n", SYM_COUR.NOM);
    exit(EXIT_FAILURE);
}





void Test_Symbole(CODES_LEX cl, CODES_ERR COD_ERR)
{
    if (SYM_COUR.CODE == cl)
    {
        Sym_Suiv();
    }
    else
        Erreur(COD_ERR);
}

void PROGRAM()
{
    Test_Symbole(PROGRAM_TOKEN, PROGRAM_ERR);
    Test_Symbole(ID_TOKEN, ID_ERR);
    // Add the program identifier to the symbol table
    addIdentifier(SYM_COUR.NOM, TPROG);
    Test_Symbole(PV_TOKEN, PV_ERR);
    BLOCK();
    GENERER1(HLT);//generation du code
    Test_Symbole(PT_TOKEN, PT_ERR);
}

void BLOCK()
{   
    //OFFSET = 0;
    CONSTS();
    
    VARS();
    PCODE[0].MNE = INT;//generation du code
    PCODE[0].SUITE = OFFSET;//generation du code
    TABS();
    INSTS();
}

void CONSTS()
{
    switch (SYM_COUR.CODE)
    {
    case CONST_TOKEN:
        Sym_Suiv();
        Test_Symbole(ID_TOKEN, ID_ERR);
        // Add the constant identifier to the symbol table
        addIdentifier(SYM_COUR.NOM, TCONST);
        TABLESYM[IND_DER_SYM_ACC ].ADRESSE = ++OFFSET;// generation code
        GENERER2(LDA, TABLESYM[IND_DER_SYM_ACC].ADRESSE);// generation code
        IND_DER_SYM_ACC++; // generation code-Réserve la mémoire pour l'ID constant
        Test_Symbole(EG_TOKEN, EG_ERR);
        CONSTVAL();
        strcpy(TABLESYM[OFFSET].NOM, SYM_COUR.NOM); // generation code
        TABLESYM[OFFSET].valeur= SYM_COUR.val;// generation code
        GENERER2(LDI, SYM_COUR.val);
        GENERER1(STO);


        while (SYM_COUR.CODE == VIR_TOKEN)
        {
            Sym_Suiv(); //const tata = 12 , toto = 13 ;
            Test_Symbole(ID_TOKEN, ID_ERR);
            // Add the constant identifier to the symbol table
        addIdentifier(SYM_COUR.NOM, TCONST);
        TABLESYM[IND_DER_SYM_ACC ].ADRESSE = ++OFFSET;// generation code
        GENERER2(LDA, TABLESYM[IND_DER_SYM_ACC].ADRESSE);// generation code
        IND_DER_SYM_ACC++; // generation code-Réserve la mémoire pour l'ID constant
        Test_Symbole(EG_TOKEN, EG_ERR);
        CONSTVAL();
        strcpy(TABLESYM[OFFSET].NOM, SYM_COUR.NOM); // generation code
        TABLESYM[OFFSET].valeur= SYM_COUR.val;// generation code
        GENERER2(LDI, SYM_COUR.val);
        GENERER1(STO);
        };
        Test_Symbole(PV_TOKEN, PV_ERR);
        break;
    case VAR_TOKEN:
        break;
    
    case BEGIN_TOKEN:
        break;
    case TYPE_TOKEN:
        break;
    default:
        Erreur(CONST_VAR_BEGIN_ERR);
        break;
    }
}
void CONSTVAL()
{
    switch (SYM_COUR.CODE)
    {
    case NUM_TOKEN:
        Test_Symbole(NUM_TOKEN, NUM_ERR);
        break;
    case REAL_TOKEN:
        Test_Symbole(REAL_TOKEN, REAL_ERR);
        break;
    case STRING_TOKEN:
        Test_Symbole(STRING_TOKEN, STRING_ERR);
        break;
    case BOOLEAN_TOKEN:
        Test_Symbole(BOOLEAN_TOKEN, BOOLEAN_ERR);
        break;

    default:
        Erreur(NUM_ERR);
        break;
    }
}

void VARS()
{
    switch (SYM_COUR.CODE)
    {
    case VAR_TOKEN:
        Sym_Suiv();
        IDLIST();
        break;
    case BEGIN_TOKEN:
        break;
    case TYPE_TOKEN:
        break;
    default:
        Erreur(VAR_BEGIN_ERR);
        break;
    }
}

void IDLIST()
{
    switch (SYM_COUR.CODE)
    {
    case ID_TOKEN:
        Sym_Suiv();
// Add the variable identifier to the symbol table
            addIdentifier(SYM_COUR.NOM, TVAR);
            TABLESYM[IND_DER_SYM_ACC ].ADRESSE = ++OFFSET;// generation code
            strcpy(TABLESYM[OFFSET].NOM, SYM_COUR.NOM); // generation code
            GENERER2(LDA, TABLESYM[IND_DER_SYM_ACC].ADRESSE);// generation code
            GENERER1(LDV);// generation code
            IND_DER_SYM_ACC++; // generation code-Réserve la mémoire pour l'ID constant
        while (SYM_COUR.CODE == VIR_TOKEN)
        {
            Sym_Suiv();
            Test_Symbole(ID_TOKEN, ID_ERR);
            // Add the variable identifier to the symbol table
            addIdentifier(SYM_COUR.NOM, TVAR);
            TABLESYM[IND_DER_SYM_ACC ].ADRESSE = ++OFFSET;// generation code
            strcpy(TABLESYM[OFFSET].NOM, SYM_COUR.NOM); // generation code
            GENERER2(LDA, TABLESYM[IND_DER_SYM_ACC].ADRESSE);// generation code
            GENERER1(LDV);// generation code
            IND_DER_SYM_ACC++; // generation code-Réserve la mémoire pour l'ID constant
        }
        Test_Symbole(DPT_TOKEN, DPT_ERR);
        TYPE();
        Test_Symbole(PV_TOKEN, PV_ERR);
        IDLIST(); // Appel récursif pour traiter les autres identifiants
        break;
    case PV_TOKEN:
        // ε - ne rien faire
        break;
    case BEGIN_TOKEN:
        break;
    case TYPE_TOKEN:
        break;
    default:
        Erreur(IDLIST_ERR);
        break;
    }
}


void TYPE()
{
    switch (SYM_COUR.CODE)
    {
    case Tinteger_TOKEN:
            Sym_Suiv();
        break;
    case Tstring_TOKEN:
            Sym_Suiv();
        break;
    case Treal_TOKEN:
            Sym_Suiv();
        break;
    case Tboolean_TOKEN:
            Sym_Suiv();
        break;
    case Tchar_TOKEN:
            Sym_Suiv();
        break;
    default:
        Erreur(TYPE_ERR);
        break;
    }
}

void TABS()
{
    switch (SYM_COUR.CODE)
    {  
    case TYPE_TOKEN:
     
        Sym_Suiv();
        Test_Symbole(ID_TOKEN, ID_ERR);
        // Add the constant identifier to the symbol table
        addIdentifier(SYM_COUR.NOM, Ttab);
        Test_Symbole(EG_TOKEN, EG_ERR);
        ARRAY();

        while (SYM_COUR.CODE == VIR_TOKEN)
        {
            Sym_Suiv();
            Test_Symbole(ID_TOKEN, ID_ERR);
        // Add the constant identifier to the symbol table
            addIdentifier(SYM_COUR.NOM, Ttab);
            Test_Symbole(EG_TOKEN, EG_ERR);
            ARRAY();
        };
        printf("%d",SYM_COUR.CODE);
        Test_Symbole(PV_TOKEN, PV_ERR);
        printf("aaaab");
        break;
    case VAR_TOKEN:
        break;
    case BEGIN_TOKEN:
    printf("aaaa");
        break;
    default:
        Erreur(CONST_VAR_BEGIN_ERR);
        break;
    }
}


void ARRAY()
{
    Test_Symbole(ARRAY_TOKEN, ARRAY_ERR);
    Test_Symbole(CO_TOKEN, CO_ERR);
 
    ELEMENT();
  
    
    while (SYM_COUR.CODE == VIR_TOKEN)
    {
        Test_Symbole(VIR_TOKEN, VIR_ERR);
        ELEMENT();
    }
    
  
    Test_Symbole(CF_TOKEN, CF_ERR);
    
    Test_Symbole(OF_TOKEN, OF_ERR);
    
    TYPE();
    
  
    printf("bnadm");
}

void ELEMENT()
{
    switch (SYM_COUR.CODE)
    {

    case STRING_TOKEN:
        Test_Symbole(STRING_TOKEN, STRING_ERR);
        break;
    case REAL_TOKEN:
        Test_Symbole(REAL_TOKEN, REAL_ERR);
        break;
    case NUM_TOKEN:
    
        Test_Symbole(NUM_TOKEN, NUM_ERR);
        Test_Symbole(DPT_TOKEN , DPT_ERR);
        Test_Symbole(NUM_TOKEN, NUM_ERR);
        // Process range (implementation needed based on your requirements)
        break;
    default:
        Erreur(ERREUR_ERR);
        break;
    }
}


void INSTS()
{
    //begin INST { ; INST } end
    Test_Symbole(BEGIN_TOKEN, BEGIN_ERR);
    semanticCheckAfterBegin();  // Check semantic rules after BEGIN
    INST();

        while (SYM_COUR.CODE == PV_TOKEN)
        {
            Sym_Suiv();
            INST();
        }
     Test_Symbole(END_TOKEN, END_ERR);
}

void INST()
{
    // INST ::= INSTS | AFFEC | SI | TANTQUE | ECRIRE | LIRE | e
    switch (SYM_COUR.CODE)
    {
    case BEGIN_TOKEN:
        INSTS();
        break;
    case ID_TOKEN:
        AFFEC();
        break;
    case IF_TOKEN:
        SI();
        break;
    case FOR_TOKEN:
        FOR();
        break;
    case WHILE_TOKEN:
        TANTQUE();
        break;
    case WRITE_TOKEN:
        ECRIRE();
        break;
    case READ_TOKEN:
        LIRE();
        break;
     case CASE_TOKEN:
        CASE();
        break;
    case PV_TOKEN:
        break;
    case END_TOKEN: 
        break;
    default:
        Erreur(VAR_ERR);
        break;
    }
}
//on doit changer ici := a des conditions ne pas affecter a des constantes

int getIdentifierIndex(char *identifier) {
    for (int i = 0; i < NbrIDFS; i++) {
        if (strcmp(TAB_IDFS[i].NOM, identifier) == 0) {
            return i; // Retourne l'index si l'identifiant est trouvé
        }
    }
    return -1; // Retourne -1 si l'identifiant n'est pas trouvé
}
void AFFEC()
{   if (!isIdentifierDeclared(SYM_COUR.NOM)) {
        Erreur(UNDECLARED_IDENTIFIER_ERR);
    }
    // Vérifier si l'identifiant est une constante on peut pas affecter a une constante
    int idIndex = getIdentifierIndex(SYM_COUR.NOM);
    if (idIndex != -1 && TAB_IDFS[idIndex].TIDF == Ttab) {
        
      ARRAY_ACCESS();

      
    }else{
           //ID := EXPR
    Test_Symbole(ID_TOKEN, ID_ERR);
    // Vérifier si l'identifiant est déjà déclaré
    if (!isIdentifierDeclared(SYM_COUR.NOM)) {
        Erreur(UNDECLARED_IDENTIFIER_ERR);
    }
    // Vérifier si l'identifiant est une constante on peut pas affecter a une constante
    int idIndex = getIdentifierIndex(SYM_COUR.NOM);
    if (idIndex != -1 && TAB_IDFS[idIndex].TIDF == TCONST) {
        // Tentative de modification d'une constante, ce qui est une erreur
        Erreur(CONST_MODIFICATION_ERR);
    }
    // Générer le code pour charger l'adresse de l'identifiant
    GENERER2(LDA, TABLESYM[idIndex-1].ADRESSE);
    }

    Test_Symbole(AFF_TOKEN, AFF_ERR);
    // Modifier l'EXPR pour gérer STRING_TOKEN --MODIF--
    
        // L'affectation standard pour les autres types (INT_TOKEN, etc.)
    EXPR();   

    // Générer le code pour l'affectation
    GENERER1(STO);

}

void SI()
{
    Test_Symbole(IF_TOKEN, IF_ERR);
    COND();
    
    GENERER2(BZE, 0);  // L'adresse cible sera complétée plus tard
    int branchPoint = PC;
    Test_Symbole(THEN_TOKEN, THEN_ERR);
    INST();
    
    if (SYM_COUR.CODE == ELSE_TOKEN)
    {
        // Gérer la partie else
        GENERER2(BRN, 0); // L'adresse cible sera complétée plus tard
        int elseBranchPoint = PC;
        PCODE[branchPoint].SUITE = PC + 1; // Adresse pour sauter si la condition est vraie
        Sym_Suiv();
        if (SYM_COUR.CODE == IF_TOKEN) {
            SI(); // Appel récursif pour traiter la partie else
            PCODE[elseBranchPoint].SUITE = PC + 1; // Adresse pour sauter à la fin du else
        } else {
            INST();
           // PCODE[elseBranchPoint].SUITE = PC + 1; // Adresse pour sauter à la fin du else

        }
    }
    else
    {
        PCODE[branchPoint].SUITE = PC + 1;
    }
}
void FOR()
{
    Test_Symbole(FOR_TOKEN, FOR_ERR);
    Test_Symbole(ID_TOKEN, ID_ERR);

    // Add the loop variable to the symbol table
    //addIdentifier(SYM_COUR.NOM, TVAR);
    TABLESYM[IND_DER_SYM_ACC].ADRESSE = ++OFFSET; // generation code
    strcpy(TABLESYM[OFFSET].NOM, SYM_COUR.NOM);   // generation code
    GENERER2(LDA, TABLESYM[IND_DER_SYM_ACC].ADRESSE); // generation code
    GENERER1(LDV);                                   // generation code
    IND_DER_SYM_ACC++;                               // generation code - Réserve la mémoire pour l'ID variable

    Test_Symbole(AFF_TOKEN, AFF_ERR);
    Test_Symbole(NUM_TOKEN, NUM_ERR);

    int debut_boucle = PC + 1; // Adresse du début de la boucle

    Test_Symbole(TO_TOKEN, TO_ERR);

    int condition_branch = PC; // Adresse de la branche conditionnelle
    Test_Symbole(NUM_TOKEN, NUM_ERR);

    GENERER2(LEQ, 0); // Comparaison pour la boucle (<=)

    Test_Symbole(DO_TOKEN, DO_ERR);
    INST();

    GENERER2(LDI, 1);         // Chargement de la valeur 1
    GENERER2(LDA, OFFSET - 1); // Chargement de l'adresse de la variable de boucle
    GENERER1(LDV);             // Valeur de la variable de boucle
    GENERER1(ADD);             // Incrémentation
    GENERER2(STO, OFFSET - 1); // Stockage de la nouvelle valeur dans la variable de boucle
    GENERER2(BRN, debut_boucle); // Saut au début de la boucle

    // Mise à jour de la branche conditionnelle
    PCODE[condition_branch].SUITE = PC + 1;
}
void VALUE()
{
    switch (SYM_COUR.CODE)
    {
    case NUM_TOKEN:
        Test_Symbole(NUM_TOKEN, NUM_ERR);
        break;
    case REAL_TOKEN:
        Test_Symbole(REAL_TOKEN, REAL_ERR);
        break;
    case STRING_TOKEN:
        Test_Symbole(STRING_TOKEN, STRING_ERR);
        break;
    case BOOLEAN_TOKEN:
        Test_Symbole(BOOLEAN_TOKEN, BOOLEAN_ERR);
        break;

    case ID_TOKEN:
        Test_Symbole(ID_TOKEN, ID_ERR);
        break;
    default:
        Erreur(ERREUR_ERR); // Handle the error case
        break;
    }
}

void CASE()
{
    Test_Symbole(CASE_TOKEN, CASE_ERR);
    EXPR();
    Test_Symbole(OF_TOKEN, OF_ERR);

    int start_of_cases = PC + 1; // Starting address of the cases

    // Generate a placeholder branch instruction
    GENERER2(BRN, 0);

    int end_of_cases = PC; // Ending address of the cases

    // Handle the CASE_BRANCHes
    do
    {
        // Generate the target address for the current CASE_BRANCH
        int target_address = PC + 2;

        // Replace the placeholder branch instruction with the correct address
        PCODE[end_of_cases].SUITE = target_address;

        // Process the VALUE
        VALUE();

        Test_Symbole(DPT_TOKEN, DPT_ERR);
        INST();

        // Generate a branch instruction to skip the remaining CASE_BRANCHes
        GENERER2(BRN, 0);

    } while (SYM_COUR.CODE == NUM_TOKEN || SYM_COUR.CODE == REAL_TOKEN || SYM_COUR.CODE == BOOLEAN_TOKEN || SYM_COUR.CODE == STRING_TOKEN || SYM_COUR.CODE == ID_TOKEN);

    // Update the branch instruction to jump to the end of the CASE statement
    PCODE[end_of_cases].SUITE = PC + 1;

    Test_Symbole(PV_TOKEN, PV_ERR);
    Test_Symbole(END_TOKEN, END_ERR);
}





void TANTQUE()
{
    Test_Symbole(WHILE_TOKEN, WHILE_ERR);
    int LABEL_BRN = PC + 1;  // Label pour l'instruction BRN
    COND();
    GENERER1(BZE);
    int INDICE_BZE = PC;
    Test_Symbole(DO_TOKEN, DO_ERR);
    INST();
    GENERER2(BRN, LABEL_BRN);
    PCODE[INDICE_BZE].SUITE=PC+1;
}

void ECRIRE()
{
    Test_Symbole(WRITE_TOKEN, WRITE_ERR);
    Test_Symbole(PO_TOKEN, PO_ERR);
    EXPR();
    GENERER1(PRN);//generation du code

    while (SYM_COUR.CODE == VIR_TOKEN)
    {
        Sym_Suiv();
        EXPR();
        GENERER1(PRN);//generation du code
    }

    Test_Symbole(PF_TOKEN, PF_ERR);
}
int isConstant(char *identifier) {
    int idIndex = getIdentifierIndex(identifier);
    return (idIndex != -1 && TAB_IDFS[idIndex].TIDF == TCONST);
}

void LIRE()
{ 
    Test_Symbole(READ_TOKEN, READ_ERR);
    Test_Symbole(PO_TOKEN, PO_ERR);
    Test_Symbole(ID_TOKEN, ID_ERR);
    // Vérifier si l'identifiant est déjà déclaré
    if (!isIdentifierDeclared(SYM_COUR.NOM)) {
        Erreur(UNDECLARED_IDENTIFIER_ERR);
    }
    // Check if the read identifier is a constant   exemple read(tata ); tq tata de type const
    if (isConstant(SYM_COUR.NOM))
    {
        Erreur(CONST_MODIFICATION_ERR);
    }

       // Récupérer l'indice de l'identifiant dans la table des symboles
        int idIndex = getIdentifierIndex(SYM_COUR.NOM);
        
        if (idIndex == -1) {
            Erreur(UNDECLARED_IDENTIFIER_ERR);
        }
        
        // Charger la valeur de l'identifiant au sommet de la pile
        GENERER2(LDA, TABLESYM[idIndex-1].ADRESSE);
        GENERER1(INN);

    while (SYM_COUR.CODE == VIR_TOKEN)
    {
        Sym_Suiv();
        Test_Symbole(ID_TOKEN, ID_ERR);

        // Récupérer l'indice de l'identifiant dans la table des symboles
        int idIndex = getIdentifierIndex(SYM_COUR.NOM);
        
        if (idIndex == -1) {
            Erreur(UNDECLARED_IDENTIFIER_ERR);
        }
        
        // Charger la valeur de l'identifiant au sommet de la pile
        GENERER2(LDA, TABLESYM[idIndex-1].ADRESSE);
        GENERER1(INN);
    }

    Test_Symbole(PF_TOKEN, PF_ERR);
}

void COND()
{   
    EXPR();
    RELOP();
      
    EXPR();
    
    // Générer le code P-Code pour l'opération de comparaison
    GENERER1(op);
}

void EXPR()
{
    //TERM { ADDOP TERM }
    TERM();

    while (SYM_COUR.CODE == PLUS_TOKEN || SYM_COUR.CODE == MOINS_TOKEN)
    {   
        // Sauvegardez l'opérateur dans une variable
        CODES_LEX operatorCode = SYM_COUR.CODE;

        ADDOP();
        TERM();

        // Générez le code pour l'opération
        if (operatorCode == PLUS_TOKEN) {
            GENERER1(ADD);
        } else if (operatorCode == MOINS_TOKEN) {
            GENERER1(SUB);
        }
    }
}

void TERM()
{
    FACT();

    while (SYM_COUR.CODE == MULT_TOKEN || SYM_COUR.CODE == DIV_TOKEN)
    {
        // Sauvegardez l'opérateur dans une variable
        CODES_LEX operatorCode = SYM_COUR.CODE;

        MULOP();
        FACT();

        // Générez le code pour l'opération
        if (operatorCode == MULT_TOKEN) {
            GENERER1(MUL);
        } else if (operatorCode == DIV_TOKEN) {
            GENERER1(DIV);
        }
    }
}

void ARRAY_ACCESS() {
    // Assuming SYM_COUR is pointing to the ID token
    Test_Symbole(ID_TOKEN, ID_ERR);

    Test_Symbole(CO_TOKEN, CO_ERR);
    // Assuming the array index is a numeric value
    Test_Symbole(NUM_TOKEN, NUM_ERR);
    Test_Symbole(CF_TOKEN, CF_ERR);
    // Load the array index onto the top of the stack
    GENERER2(LDI, SYM_COUR.val);

}

void FACT()
{
    switch (SYM_COUR.CODE)
    {
    case ID_TOKEN:
        
    if (!isIdentifierDeclared(SYM_COUR.NOM)) {
        Erreur(UNDECLARED_IDENTIFIER_ERR);
    }
    // Vérifier si l'identifiant est une constante on peut pas affecter a une constante
    int idIndex = getIdentifierIndex(SYM_COUR.NOM);
    if (idIndex != -1 && TAB_IDFS[idIndex].TIDF == Ttab) {
        
      ARRAY_ACCESS();
      break;
      
    }else{
        Test_Symbole(ID_TOKEN, ID_ERR);
        // Récupérer l'indice de l'identifiant dans la table des symboles
        int idIndex = getIdentifierIndex(SYM_COUR.NOM);
        if (idIndex == -1) {
            Erreur(UNDECLARED_IDENTIFIER_ERR);
        }
        // Charger la valeur de l'identifiant au sommet de la pile
        GENERER2(LDA, TABLESYM[idIndex-1].ADRESSE);
        GENERER1(LDV);
        break;
    }
    case NUM_TOKEN:
        Test_Symbole(NUM_TOKEN, NUM_ERR);
        // Charger la valeur numérique au sommet de la pile
        GENERER2(LDI, SYM_COUR.val);
        break;
    case REAL_TOKEN :
        Test_Symbole(REAL_TOKEN , REAL_ERR);
                 // Charger la valeur numérique au sommet de la pile
        GENERER2(LDI, SYM_COUR.val);
        break;
    case TRUE_TOKEN:
        Test_Symbole(TRUE_TOKEN , TRUE_ERR);
        // Handle true token
        GENERER2(LDI, 1);  // Assuming true is represented as 1
        break;
    case FALSE_TOKEN:
        Test_Symbole(FALSE_TOKEN , FALSE_ERR);
        // Handle false token
        GENERER2(LDI, 0);  // Assuming false is represented as 0
        break;
    case STRING_TOKEN:
        Test_Symbole(STRING_TOKEN ,STRING_ERR);
        // Handle character token
        GENERER2(LDI, (int)SYM_COUR.val);  // Assuming char is represented as an integer
        break;
    case PO_TOKEN:
        printf("%d U5UP",SYM_COUR.CODE);
        Test_Symbole(PO_TOKEN, PO_ERR);
        EXPR();
        Test_Symbole(PF_TOKEN, PF_ERR);
        break;  

    default:
        Erreur(ERREUR_ERR);
        break;
    }
}
// tab[3]:=8;      a:=  tab[3];

void RELOP()
{   
    
    switch (SYM_COUR.CODE)
    {
    case EG_TOKEN:
        Test_Symbole(SYM_COUR.CODE, EG_ERR);
        op = EQL;
        break;
    case DIFF_TOKEN:
        Test_Symbole(SYM_COUR.CODE, DIFF_ERR);
        op = NEQ;
    case INF_TOKEN:
        Test_Symbole(SYM_COUR.CODE, INF_ERR);
        op = LSS;
    case SUP_TOKEN:
        Test_Symbole(SYM_COUR.CODE, SUP_ERR);
        op = GTR;
    case INFEG_TOKEN:
        Test_Symbole(SYM_COUR.CODE, INFEG_ERR);
        op =  LEQ;
    case SUPEG_TOKEN:
        Test_Symbole(SYM_COUR.CODE, SUPEG_ERR);
        op = GEQ;
        break;
    default:
        Erreur(ERREUR_ERR);
        break;
    }
}

void ADDOP()
{
    switch (SYM_COUR.CODE)
    {
    case PLUS_TOKEN:
        Test_Symbole(SYM_COUR.CODE, PLUS_ERR);
        //GENERER1(ADD);
        break;
    case MOINS_TOKEN:
        Test_Symbole(SYM_COUR.CODE, MOINS_ERR);
        //GENERER1(SUB);
        break;
    default:
        Erreur(ERREUR_ERR);
        break;
    }
}

void MULOP()
{
    switch (SYM_COUR.CODE)
    {
    case MULT_TOKEN:
        Test_Symbole(SYM_COUR.CODE, MULT_ERR);
        //GENERER1(MUL);
        break;
    case DIV_TOKEN:
        Test_Symbole(SYM_COUR.CODE, DIV_ERR);
        //  GENERER1(DIV);
        break;
    default:
        Erreur(ERREUR_ERR);
        break;
    }
}

void displayUsedIdentifiers() {
    printf("List of Used Identifiers:\n");
    printf("-------------------------------------------------\n");
    for (int i = 0; i < NbrIDFS; ++i) {
        printf("| %-20s | %-20d |\n", TAB_IDFS[i].NOM,TAB_IDFS[i].TIDF);
    }
    printf("-------------------------------------------------\n");
}
void afficherTableSymboles() {
    printf("\nTable des symboles :\n");
    printf("-----------------------------------------------------------------------\n");
    printf("| %-20s | %-20s | %-20s | \n", "Nom","valeur", "Adresse");
    printf("-----------------------------------------------------------------------\n");

    for (int i = 0; i <= OFFSET; ++i) {
        printf("| %-20s | %-20d | %-20d |\n", TABLESYM[i].NOM, TABLESYM[i].valeur, TABLESYM[i].ADRESSE);
    }

    printf("-----------------------------------------------------------------------\n");
}

const char *mnemoniques[] = {
    "ADD", "SUB", "MUL", "DIV", "EQL", "NEQ", "GTR", "LSS", "GEQ", "LEQ", 
    "PRN", "INN", "INT", "LDI", "LDA", "LDV", "STO", "BRN", "BZE", "HLT"
};

void afficherPCode() {
    printf("Table P-Code:\n");
    for (int i = 0; i < TAILLECODE; i++) {
        printf(" | %-20s | %-20d |\n", mnemoniques[PCODE[i].MNE], PCODE[i].SUITE);
    }
    printf("\n");
}


//=================================================================================
//=================================================================================
//p-code dans fichier
FILE *FICH_SORTIE;

void SaveInstToFile(INSTRUCTION INST)
{
switch( INST.MNE){
case LDA: fprintf(FICH_SORTIE, "%s \t %d \n", "LDA", INST.SUITE); break;
case LDI: fprintf(FICH_SORTIE, "%s \t %d \n", "LDI", INST.SUITE); break;
case INT: fprintf(FICH_SORTIE, "%s \t %d \n", "INT", INST.SUITE); break;
case BZE: fprintf(FICH_SORTIE, "%s \t %d \n", "BZE", INST.SUITE); break;
case BRN: fprintf(FICH_SORTIE, "%s \t %d \n", "BRN", INST.SUITE); break;
case LDV: fprintf(FICH_SORTIE, "%s \n", "LDV"); break;
case ADD: fprintf(FICH_SORTIE, "%s \n", "ADD"); break;
case SUB: fprintf(FICH_SORTIE, "%s \n", "SUB"); break;
case MUL: fprintf(FICH_SORTIE, "%s \n", "MUL"); break;
case DIV: fprintf(FICH_SORTIE, "%s \n", "DIV"); break;
case LEQ: fprintf(FICH_SORTIE, "%s \n", "LEQ"); break;
case GEQ: fprintf(FICH_SORTIE, "%s \n", "GEQ"); break;
case NEQ: fprintf(FICH_SORTIE, "%s \n", "NEQ"); break;
case LSS: fprintf(FICH_SORTIE, "%s \n", "LSS"); break;
case GTR: fprintf(FICH_SORTIE, "%s \n", "GTR"); break;
case HLT: fprintf(FICH_SORTIE, "%s \n", "HLT"); break;
case STO: fprintf(FICH_SORTIE, "%s \n", "STO"); break;
case INN: fprintf(FICH_SORTIE, "%s \n", "INN"); break;
case PRN: fprintf(FICH_SORTIE, "%s \n", "PRN"); break;
case EQL: fprintf(FICH_SORTIE, "%s \n", "EQL"); break;
default: Erreur(INST_PCODE_ERR); break;
}
}

//=================================================================================
//=================================================================================

int main()
{
    fichier = fopen("exemple.txt", "r");
    if (fichier == NULL)
    {
        perror("Erreur lors de l'ouverture du fichier");
        return 1;
    }

    Lire_Car();
    Sym_Suiv();

    PROGRAM();

    printf("Program execution completed.\n");
    
    if (SYM_COUR.CODE == FIN_TOKEN)
    {
        printf("BRAVO: le programme est correcte!!!\n");
    }
    else
    {
        printf("PAS BRAVO: fin de programme erronée!!!!\n");
    }

    displayUsedIdentifiers();
    afficherTableSymboles();
    afficherPCode();


//=================================================================================
//=================================================================================
FICH_SORTIE=fopen("fichierSortie.op", "w+" );


int i;
for (i=0; i<=PC; i++) {
    SaveInstToFile(PCODE[i]);
    }

fclose(FICH_SORTIE);
//=================================================================================
//=================================================================================

    fclose(fichier);

    return 0;
}