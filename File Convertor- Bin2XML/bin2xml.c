#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdio.h>
#include <limits.h>
#include <wctype.h>


struct record {

    char name[64]; //utf16
    char surname[32]; //utf8
    char gender;
    char email[32];
    char phone_number[16];
    char address[32];
    char level_of_education[8];
    unsigned int income_level; // given little-endian
    unsigned int expenditure; // given big-endian
    char currency_unit[16];
    char currentMood[32];
    float height;
    unsigned int weight;
};


unsigned int littleToBig(int x){
    return (((x>>24) & 0x000000ff) | ((x>>8) & 0x0000ff00) | ((x<<8) & 0x00ff0000) | ((x<<24) & 0xff000000) );
}


void createXSD(char *nameOfRoot){


    FILE *file;



    const char *patternForNameAndSurname = "[A-Za-z]+";
    const char *patternForGender = "(M|F)";
    const char *patternForPhoneNumber = "^[0-9]{3}-[0-9]{3}-[0-9]{4}$";
    const char *patternForEducationLevel = "(PhD|MSc|BSc|HS|PS)";
    const char *patternForCurrency= "(€|?|$)";
    const char *patternForEmail= "/^\\w+([\\.-]?\\w+)*@\\w+([\\.-]?\\w+)*(\\.\\w{2,3})+$/";
    const char *patternForAddress = "[.]+";


    file = fopen(nameOfRoot,"w");




    fprintf(file,"<?xml version=\"1.0\" encoding=\"utf-8\"?>\n");
    fprintf(file,"\t<xs:element name=\"%s\">\n",nameOfRoot);
    fprintf(file,"\t\t<xs:complexType>\n");
    fprintf(file,"\t\t\t<xs:sequence>\n");
    fprintf(file,"\t\t\t\t<xs:element maxOccurs=\"unbounded\" name=\"row\">\n");
    fprintf(file,"\t\t\t\t\t<xs:complexType>\n");
    fprintf(file,"\t\t\t\t\t\t<xs:sequence>\n");
    fprintf(file,"\t\t\t\t\t\t\t<xs:element name=\"%s\" type=\"xs:pattern %s\" />""\n","name",patternForNameAndSurname);
    fprintf(file,"\t\t\t\t\t\t\t<xs:element name=\"%s\" type=\"xs:pattern %s\" />""\n","surname",patternForNameAndSurname);
    fprintf(file,"\t\t\t\t\t\t\t<xs:element name=\"%s\" type=\"xs:pattern %s\" />""\n","gender",patternForGender);
    fprintf(file,"\t\t\t\t\t\t\t<xs:element name=\"%s\" type=\"xs:pattern %s\" />""\n","email",patternForEmail);
    fprintf(file,"\t\t\t\t\t\t\t<xs:element name=\"%s\" type=\"xs:pattern %s\" />""\n","phone_number",patternForPhoneNumber);
    fprintf(file,"\t\t\t\t\t\t\t<xs:element name=\"%s\" type=\"xs:pattern %s\" />""\n","address",patternForAddress);
    fprintf(file,"\t\t\t\t\t\t\t<xs:element name=\"%s\" type=\"xs:pattern %s\" />""\n","level_of_education",patternForEducationLevel);
    fprintf(file,"\t\t\t\t\t\t\t<xs:element name=\"%s\" type=\"xs:pattern %s\" />""\n","currency_unit",patternForCurrency);
    fprintf(file,"\t\t\t\t\t\t\t<xs:element name=\"%s\" type=\"xs:decimal\" />""\n","height");
    fprintf(file,"\t\t\t\t\t\t\t<xs:element name=\"%s\" type=\"xs:unsignedByte\" />""\n","weight");
    fprintf(file,"\t\t\t\t\t\t</xs:sequence>\n");
    fprintf(file,"\t\t\t\t\t\t<xs:attribute name=\"id\" type=\"xs:unsignedByte\" use=\"required\" />\n");
    fprintf(file,"\t\t\t\t\t</xs:complexType>\n");
    fprintf(file,"\t\t\t\t</xs:element>\n");
    fprintf(file,"\t\t\t</xs:sequence>\n");
    fprintf(file,"\t\t</xs:complexType>\n");
    fprintf(file,"\t</xs:element>\n");
    fprintf(file,"</xs:schema>\n");


    fclose(file);


}





void validate(char *xsdFile, char* xmlFile){

    bool validateCompleted = true;


    char *word1XSD = "<?xml version=\"1.0\" encoding=\"utf-8\"?>";
    char *word2XSD = "<xs:complexType>";
    char *word3XSD = "<xs:sequence>";
    char *word4XSD = "</xs:schema>";
    char *word5XSD = "</xs:sequence>";
    char *word6XSD = "</xs:complexType>";
    char *word7XSD = "</xs:element>";
    char *word8XSD = "</xs:sequence>";
    char *word9XSD = "</xs:schema>";
    char *word10XSD = "<xs:element name=\"records\">";
    char *word11XSD = "<xs:element maxOccurs=\"unbounded\" name=\"row\">";
    char *word12XSD = "<xs:attribute name=\"id\" type=\"xs:unsignedByte\" use=\"required\" />";


    char lineXSD[100];

    int count = 0;
    char arrTag[13][50];
    char arrTag2[9][50];

    FILE *fileXSD;
    fileXSD = fopen(xsdFile, "r");


    if(fileXSD != NULL){

        while (fgets(lineXSD, sizeof(lineXSD), fileXSD)) {


            char *address1 = strstr(lineXSD,word1XSD);
            char *address2 = strstr(lineXSD,word2XSD);
            char *address3 = strstr(lineXSD,word3XSD);
            char *address4 = strstr(lineXSD,word4XSD);
            char *address5 = strstr(lineXSD,word5XSD);
            char *address6 = strstr(lineXSD,word6XSD);
            char *address7 = strstr(lineXSD,word7XSD);
            char *address8 = strstr(lineXSD,word8XSD);
            char *address9 = strstr(lineXSD,word9XSD);
            char *address10 = strstr(lineXSD,word10XSD);
            char *address11 = strstr(lineXSD,word11XSD);
            char *address12 = strstr(lineXSD,word12XSD);



            if(address1 || address2 || address3 || address4 || address5 || address6 || address7 || address8 || address9 || address10 || address11 || address12 ){
                continue;
            }
            else{

                char *result = strstr(lineXSD, "name=\"");
                int position = result - lineXSD + 6;

                int i;

                char temp[40] = "";
                for(i = position; i<strlen(lineXSD); i++){
                    if(isalpha(lineXSD[i]) == 0 && lineXSD[i] != '_'){
                        break;
                    }
                    else{
                        strncat(temp,&lineXSD[i],1);

                    }

                }

                strcpy(arrTag[count], temp);


                char *result2 = strstr(lineXSD, "\"xs:pattern ");
                int position2 = result2 - lineXSD + 12;

                int j;

                char temp2[40] = "";
                for (j = position2; j < strlen(lineXSD); j++) {
                    if (lineXSD[j] == '"') {
                        break;
                    } else {
                        strncat(temp2, &lineXSD[j], 1);

                    }

                }
                strcpy(arrTag2[count], temp2);

            }

            count++;
        }



    }else{
        printf("%s", "does not exist");
    }



    fclose(fileXSD);



    FILE *file;


    char *word1 = "<records>";
    char *word2 = "<row id=";
    char *word3 = "</row>";
    char *word4 = "</records>";




    file = fopen(xmlFile, "r");

    char line[100];

    if(file != NULL){
        int j=0;
        while (fgets(line, sizeof(line), file)) {

            char *address1 = strstr(line,word1);
            char *address2 = strstr(line,word2);
            char *address3 = strstr(line,word3);
            char *address4 = strstr(line,word4);

            if(address1 || address2 || address3 || address4){
                continue;
            }
            else{
                char arr[3][30];
                char string[100];
                strcpy(string,line);
                char * token = strtok(string, ">");
                strcpy(arr[0],token);
                int indexA = 1;
                while( token != NULL ) {
                    token = strtok(NULL, "<");
                    if(token != NULL){
                        strcpy(arr[indexA],token);
                        indexA++;
                    }
                }

                int i;

                for(i = 0; i<3; i=i+2){
                    int a;
                    char temp[20] = "";
                    for(a = 0; a<strlen(arr[i]);a++){
                        if(isalpha(arr[i][a]) == 0 && arr[i][a] != '_'){
                            continue;
                        }
                        else{

                            strncat(temp,&arr[i][a],1);

                        }
                    }

                    strcpy(arr[i],temp);

                }



                if(arr[1][0] == '/'  || strcmp(arr[1],"0") == 0 || strcmp(arr[1],"0.000000") == 0 || strcmp(arr[1]," ") == 0 ){
                    continue;
                }

                if(strcmp(arr[0],arr[2]) != 0){
                    printf("Unmatched Tags Error => Open Tag: %s Close Tag: %s\n", arr[0],arr[2]);
                    validateCompleted = false;

                }else{

                    char fieldName[50];
                    bool found = false;
                    int xsdIndex;
                    for(xsdIndex = 0; xsdIndex < sizeof(arrTag)/sizeof(arrTag[0]); xsdIndex++){

                        if(strcmp(arrTag[xsdIndex], arr[0]) == 0){
                            found = true;
                            strcpy(fieldName,arrTag[xsdIndex]);
                            break;
                        }
                    }

                    if(found == false){
                        printf("Tag does not exist in XSD: %s\n", arr[0]);
                        validateCompleted = false;
                    }else{
                        short blankCount = 0;
                        if(strcmp(fieldName, "name") == 0 || strcmp(fieldName, "surname") == 0){
                            if(strcmp(arrTag2[1],"[A-Za-z]+") == 0){
                                int fieldNameIndex;
                                for(fieldNameIndex = 0; fieldNameIndex< strlen(arr[1]); fieldNameIndex++){
                                    if(arr[1][fieldNameIndex] == ' '){
                                        blankCount++;
                                    }
                                    else if(iswalpha(arr[1][fieldNameIndex]) == 0 || blankCount>1){
                                        printf("Invalid data type for <%s> attribute, the word is %s\n", fieldName, arr[1]);
                                        validateCompleted = false;
                                        break;
                                    }
                                }
                            }
                        }
                        else if(strcmp(fieldName, "gender") == 0){

                            char first;
                            char second;

                            first = arrTag2[3][1];
                            second = arrTag2[3][3];


                            int fieldNameIndex;
                            for(fieldNameIndex = 0; fieldNameIndex< strlen(arr[1]); fieldNameIndex++){
                                if((arr[1][fieldNameIndex] != first) && ( arr[1][fieldNameIndex] != second) ){
                                    printf("Invalid data type for <%s> attribute, the word is %s\n", fieldName, arr[1]);
                                    validateCompleted = false;
                                    break;
                                }
                            }

                        }
                        else if(strcmp(fieldName, "email") == 0){
                            if(strcmp(arrTag2[4],"/^w+([.-]?w+)*@w+([.-]?w+)*(.w{2,3})+$/") == 0) {
                                char copyOfArr1[] = "";
                                strcpy(copyOfArr1, arr[1]);
                                char *token2 = strtok(arr[1], "@");
                                token2 = strtok(NULL, ".");
                                int tokenIndex;
                                for (tokenIndex = 0; tokenIndex < strlen(token2); tokenIndex++) {
                                    if (isalpha(token2[tokenIndex]) == 0) {
                                        printf("Invalid data type for <%s> attribute, the word is %s\n", fieldName,
                                               copyOfArr1);
                                        validateCompleted = false;
                                        break;
                                    }
                                }
                                token2 = strtok(NULL, "");

                                if (strcmp(token2, "com") != 0) {
                                    printf("Invalid data type for <%s> attribute, the word is %s\n", fieldName,
                                           copyOfArr1);
                                    validateCompleted = false;
                                }

                            }


                        }else if(strcmp(fieldName, "phone_number") == 0) {

                            if (strcmp(arrTag2[6], "^[0-9]{3}-[0-9]{3}-[0-9]{4}$") == 0) {
                                int phoneIndex;

                                if (strlen(arr[1]) != 12) {
                                    printf("Invalid phone number: %s\n", arr[1]);
                                    validateCompleted = false;
                                } else {


                                    for (phoneIndex = 0; phoneIndex < strlen(arr[1]); phoneIndex++) {

                                        if ((phoneIndex == 3 || phoneIndex == 7) && arr[1][phoneIndex] != '-') {
                                            printf("Invalid data type for <%s> attribute, the word is %s\n", fieldName,
                                                   arr[1]);
                                            validateCompleted = false;
                                            break;
                                        } else if (isdigit(arr[1][phoneIndex]) == 0 && arr[1][phoneIndex] != '-') {
                                            printf("Invalid data type for <%s> attribute, the word is %s\n", fieldName,
                                                   arr[1]);
                                            validateCompleted = false;
                                            break;
                                        }
                                    }

                                }

                            }
                        }

                        else if(strcmp(fieldName, "level_of_education") == 0){

                            if(strcmp(arr[1], "PhD" )  != 0 && strcmp(arr[1], "MSc" )  != 0 && strcmp(arr[1], "BSc" )  != 0
                               && strcmp(arr[1], "HS" )  != 0 && strcmp(arr[1], "PS" )  != 0){
                                validateCompleted = false;
                                printf("Invalid data type for <%s> attribute, the word is %s\n",fieldName, arr[1]);

                            }

                        }
                        else if(strcmp(fieldName, "currency_unit") == 0){

                            if(strcmp(arr[1], "₺" )  != 0 && strcmp(arr[1], "$" )  != 0 && strcmp(arr[1], "€" )  != 0){

                                printf("Invalid data type for <%s> attribute, the word is %s\n",fieldName, arr[1]);
                                validateCompleted = false;

                            }

                        }
                        else if(strcmp(fieldName, "height") == 0){
                            int heightIndex;


                            for (heightIndex = 0; heightIndex < strlen(arr[1]); heightIndex++) {

                                if ((heightIndex == 1) && arr[1][heightIndex] != '.') {
                                    printf("Invalid data type for <%s> attribute, the word is %s\n",fieldName, arr[1]);
                                    validateCompleted = false;
                                    break;
                                } else if (isdigit(arr[1][heightIndex]) == 0 && arr[1][heightIndex] != '.') {
                                    printf("Invalid data type for <%s> attribute, the word is %s\n",fieldName, arr[1]);
                                    validateCompleted = false;
                                    break;
                                }
                            }

                        }
                        else if(strcmp(fieldName, "weight") == 0){
                            int weightIndex;

                            for (weightIndex = 0; weightIndex < strlen(arr[1]); weightIndex++) {

                                if (isdigit(arr[1][weightIndex]) == 0) {
                                    printf("Invalid data type for <%s> attribute, the word is %s\n",fieldName, arr[1]);
                                    validateCompleted = false;
                                    break;
                                }
                            }

                        }





                    }
                }







            }





        }

    }

    else{
        printf("%s", "File does not exist");
    }


    if(validateCompleted){
        printf("Validation Completed.");
    }else{
        printf("Validation Error.");
    }


    fclose(file);




}





void xmlCreator(char input[], char output[] ){




    FILE *file;
    FILE *fileUTF16;
    FILE *file2;


    file = fopen(input, "rb");

    file2 = fopen(output, "w");

    fileUTF16 = fopen(input, "rb, ccs=UTF-16LE");


    struct record r1;
    struct record r2;
    char nameOfRoot[50] = "";
    int index;
    for(index=0; index<strlen(output);index++){
        if(output[index] == '.'){
            break;
        }
        strncat(nameOfRoot,&output[index],1);
    }

    fprintf(file2,"<%s>\n\t","records");

    if(file !=NULL){
        int i =0;
        int rowId = 1;
        while(fread(&r1,sizeof(struct record),1,file)!=NULL){
            if(i>0){
                if(i == 1){
                    fread(&r2,sizeof(struct record),1,fileUTF16);
                    fread(&r1,64,1,fileUTF16);
                }else{
                    int indexForUTF = 0;
                    while(indexForUTF<i) {
                        fread(&r2,sizeof(struct record),1,fileUTF16);
                        indexForUTF++;
                    }
                    fread(&r1, 64, 1, fileUTF16);
                }

                fclose(fileUTF16);
                int nameIndex;
                char newName[] = "";
                for(nameIndex = 0; nameIndex< strlen(r1.name);nameIndex++){
                    if(iswalpha(r1.name[nameIndex]) == 0) {
                        break;
                    }
                    strncat(newName,&r1.name[nameIndex],1);
                }




                fprintf(file2,"<row id=\"%d\">\n\t\t",i);
                fprintf(file2,"<name>%s</name>\n\t\t",newName);
                fprintf(file2,"<surname>%s</surname>\n\t\t",r1.surname);
                fprintf(file2,"<gender>%c</gender>\n\t\t",r1.gender);
                fprintf(file2,"<email>%s</email>\n\t\t",r1.email);
                fprintf(file2,"<phone_number>%s</phone_number>\n\t\t",r1.phone_number);
                fprintf(file2,"<address>%s</address>\n\t\t",r1.address);
                fprintf(file2,"<level_of_education>%s</level_of_education>\n\t\t",r1.level_of_education);
                fprintf(file2,"<currency_unit>%s</currency_unit>\n\t\t",r1.currency_unit);
                fprintf(file2,"<height>%f</height>\n\t\t",r1.height);
                fprintf(file2,"<weight>%d</weight>\n\t",r1.weight);
                fprintf(file2,"</row>\n",i);


                r1.expenditure = littleToBig(r1.expenditure);
                //printf("Income level: %d\n", r1.income_level);
                //printf("expenditure: %d\n", r1.expenditure);

            }
            i++;
            rowId++;
        }

        fprintf(file2,"</%s>",nameOfRoot);



    }else{
        printf("File does not exist!");
    }


    fclose(file2);
    fclose(file);

}


void Bin2XML(char *inputfile, char *outputfile){

    createXSD("records.xsd");
    xmlCreator(inputfile,outputfile);
    validate("records.xsd",outputfile);
}




int main(int argc, char *argv[]){

    if(strcmp(argv[0],"Bin2XML")==0){
        Bin2XML(argv[1],argv[2]);
    }


    return 0;

}


