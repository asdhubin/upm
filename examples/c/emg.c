/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2016 Intel Corporation.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include <unistd.h>
#include <signal.h>

#include "emg.h"

bool shouldRun = true;

void sig_handler(int signo)
{
    if (signo == SIGINT)
        shouldRun = false;
}

int main()
{
    signal(SIGINT, sig_handler);

    //! [Interesting]

    // Instantiate a emg sensor on analog pin A0
    emg_context sensor = emg_init(0);

    if (!sensor)
    {
        printf("emg_init() failed.\n");
        return -1;
    }

    // Set the aref, scale, and offset
    emg_set_aref(sensor, 5.0);
    emg_set_scale(sensor, 1.0);
    emg_set_offset(sensor, -.1);
    printf("aRef: %0.03f scale: %0.03f offset: %0.03f\n\n",
            emg_get_aref(sensor),
            emg_get_scale(sensor),
            emg_get_offset(sensor));

    // Every half a second, sample the sensor output
    while (shouldRun)
    {
        float normalized = 0.0;
        float raw_volts = 0.0;
        float volts = 0.0;

        emg_get_normalized(sensor, &normalized);
        emg_get_raw_volts(sensor, &raw_volts);
        emg_get_volts(sensor, &volts);

        printf("Normalized output: %0.03f, raw emg sensor output: %0.03f v "
                "adjusted output: %0.03f v\n", normalized, raw_volts, volts);

        usleep(500000);
    }

    //! [Interesting]

    printf("Exiting\n");

    emg_close(sensor);

    return 0;
}
