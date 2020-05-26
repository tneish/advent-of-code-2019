#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>


void day1_1(const char* in) 
{
    const char delim[] = "\n";
    char *r;
    char *end;
    char *tk;
    r = strdup(in);
    end = r;

    int total_fuel = 0;
    tk = strsep (&end, delim);
    while (tk != NULL) {
        float f = strtof(tk,NULL);
        f = f / 3;
        int x = (int)f;
        x -= 2;
        total_fuel += x;
        tk = strsep (&end, delim);
    }
    printf("%d\n", total_fuel);
    free(r);
}

void day1_2(const char* in) 
{
    const char delim[] = "\n";
    char *r;
    char *end;
    char *tk;
    float fuel;

    r = strdup(in);
    end = r;

    int tot_fuel = 0;
    tk = strsep(&end, delim);
    while (tk != NULL) {
        // Module mass
        float mass = strtof(tk, NULL);

        fuel = 0;
        while (1) {
            fuel = mass / 3;
            int x = (int)fuel;
            x -= 2;
            if (x <= 0) {
                break;
            }
            tot_fuel += x;
            mass = (float)x;
        }

        tk = strsep(&end, delim);
    }
    
    printf("%d\n", tot_fuel);
    free(r);
}

int main (int argc, char *argv[]) 
{
    const char day1_in[] = "123835\n66973\n63652\n99256\n56009\n58012\n130669\n109933\n52958\n131656\n144786\n50437\n134194\n80230\n50326\n118204\n102780\n135520\n142248\n80341\n51071\n71346\n134081\n142321\n136230\n55934\n79697\n90116\n107825\n133052\n130259\n99566\n83066\n90923\n58475\n134697\n91830\n105838\n109003\n125258\n108679\n87310\n79813\n109814\n65616\n69275\n118405\n105178\n93140\n79535\n138051\n55728\n71875\n121207\n52011\n81209\n129059\n135782\n62791\n72135\n77765\n109498\n73862\n134825\n148898\n81633\n53277\n109858\n91672\n115105\n132871\n138334\n135049\n73083\n79234\n129281\n86062\n88448\n99612\n52138\n149290\n120562\n118975\n92896\n51162\n122410\n75479\n137800\n142149\n123518\n67806\n89937\n85963\n104764\n56710\n51314\n67275\n61135\n77580\n74726";

    printf("--- Day 1 ---\n");
    day1_1(day1_in);  // 3226822
    day1_2(day1_in);  // 4837367

    return EXIT_SUCCESS;

}