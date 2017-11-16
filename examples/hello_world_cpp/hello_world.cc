/* -*- Mode: C; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

/*  Fluent Bit Demo
 *  ===============
 *  Copyright (C) 2015 Treasure Data Inc.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#include <fluent-bit.h>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <unistd.h>

using namespace std;

int main()
{
    int i;
    int n;
    int ct;
    int ret;
    char tmp[256];
    flb_ctx_t *ctx;
    int in_ffd;
    int out_ffd;

    /* Initialize library */
    ctx = flb_create();

    ofstream myfile;

    myfile.open("/mylog.txt");
    for (i = 0; i < 100; i++) {
        myfile << "Log line" << i << endl;
    }

    in_ffd = flb_input(ctx, (char *) "tail", NULL);
    int ret_val = flb_input_set(ctx, in_ffd,
                                "path", "/mylog.txt",
                                "tag", "debug.55",
                                NULL);
    if (ret_val != 0) {
        throw runtime_error("error setting input param");
    }

    out_ffd = flb_output(ctx, (char *) "stdout", NULL);
    flb_output_set(ctx, out_ffd, (char *) "test", NULL);

    // out_ffd = flb_output(ctx, (char *) "forward", NULL);
    // ret_val = flb_output_set(ctx, out_ffd,
    //                          "host", "10.0.9.61",
    //                          "hostname", "osv.local",
    //                          "port", "24224",
    //                          "time_as_integer", "true",
    //                          "tls", "on",
    //                          "tls.verify", "off",
    //                          "Shared_Key", "secret",
    //                          NULL);
    // if (ret_val != 0) {
    //     throw runtime_error("error setting input param");
    // }
    // flb_output_set(ctx, out_ffd, (char *) "test", NULL);

    /* Start the background worker */
    flb_start(ctx);

    ct = 100;
    cout << "got to here\n";
    for (;;) {
        for (int j = 0; j < 10; j++) {
            myfile << "Log line" << ct << endl;
            cout << "Writing " << ct << endl;
            ++ct;
            sleep(1);
        }
        myfile << "Wrapped" << ct << endl;
        myfile.flush();
    }
    myfile.close();

    flb_stop(ctx);

    /* Release Resources */
    flb_destroy(ctx);


    return 0;
}
