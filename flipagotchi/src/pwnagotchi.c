#include "../include/pwnagotchi.h"

/*
Icons from RogueMaster at:
https://github.com/RogueMaster/flipperzero-firmware-wPlugins/commit/8c45f8e9a921f61cda78ecdb2e58a244041d3e05
*/
#include "flipagotchi_icons.h"

#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <furi.h>

Pwnagotchi* pwnagotchi_alloc() {
    Pwnagotchi* pwn = malloc(sizeof(Pwnagotchi));

    // Set numbered values
    pwn->face = Cool;
    pwn->friendFace = NoFace;
    pwn->mode = PwnMode_Manual;

    // Set string values to initial
    pwn->channel = furi_string_alloc_set_str(CHANNEL_DEFAULT_TEXT);
    pwn->apStat = furi_string_alloc_set_str(AP_STAT_DEFAULT_TEXT);
    pwn->uptime = furi_string_alloc_set_str(UPTIME_DEFAULT_TEXT);
    pwn->hostname = furi_string_alloc_set_str(HOSTNAME_DEFAULT_TEXT);
    pwn->message = furi_string_alloc_set_str(MESSAGE_DEFAULT_TEXT);
    pwn->handshakes = furi_string_alloc_set_str(HANDSHAKES_DEFAULT_TEXT);
    pwn->friendStat = furi_string_alloc_set_str(FRIEND_STAT_DEFAULT_TEXT);

    return pwn;
}

void pwnagotchi_draw_face(Pwnagotchi* pwn, Canvas* canvas) {
    const Icon* currentFace;
    bool draw = true;

    switch(pwn->face) {
    case NoFace:
        // Draw nothing
        draw = false;
        break;
    case DefaultFace:
        currentFace = &I_awake_flipagotchi;
        break;
    case Look_r:
        currentFace = &I_look_r_flipagotchi;
        break;
    case Look_l:
        currentFace = &I_look_l_flipagotchi;
        break;
    case Look_r_happy:
        currentFace = &I_look_r_happy_flipagotchi;
        break;
    case Look_l_happy:
        currentFace = &I_look_l_happy_flipagotchi;
        break;
    case Sleep:
        currentFace = &I_sleep_flipagotchi;
        break;
    case Sleep2:
        currentFace = &I_sleep2_flipagotchi;
        break;
    case Awake:
        currentFace = &I_awake_flipagotchi;
        break;
    case Bored:
        currentFace = &I_bored_flipagotchi;
        break;
    case Intense:
        currentFace = &I_intense_flipagotchi;
        break;
    case Cool:
        currentFace = &I_cool_flipagotchi;
        break;
    case Happy:
        currentFace = &I_happy_flipagotchi;
        break;
    case Grateful:
        currentFace = &I_grateful_flipagotchi;
        break;
    case Excited:
        currentFace = &I_excited_flipagotchi;
        break;
    case Motivated:
        currentFace = &I_motivated_flipagotchi;
        break;
    case Demotivated:
        currentFace = &I_demotivated_flipagotchi;
        break;
    case Smart:
        currentFace = &I_smart_flipagotchi;
        break;
    case Lonely:
        currentFace = &I_lonely_flipagotchi;
        break;
    case Sad:
        currentFace = &I_sad_flipagotchi;
        break;
    case Angry:
        currentFace = &I_angry_flipagotchi;
        break;
    case Friend:
        currentFace = &I_friend_flipagotchi;
        break;
    case Broken:
        currentFace = &I_broken_flipagotchi;
        break;
    case Debug:
        currentFace = &I_debug_flipagotchi;
        break;
    case Upload:
        currentFace = &I_upload_flipagotchi;
        break;
    case Upload1:
        currentFace = &I_upload1_flipagotchi;
        break;
    case Upload2:
        currentFace = &I_upload2_flipagotchi;
        break;
    default:
        draw = false;
    }

    if(draw) {
        canvas_draw_icon(canvas, PWNAGOTCHI_FACE_J, PWNAGOTCHI_FACE_I, currentFace);
    }
}

/**
 * Function to draw a generic string with a preceder and data
 * 
 * @param preceder Preceding characters in the string
 * @param info Actual string from pwnagotchi to print
 * @param i_pos Position in the i direction to start printing
 * @param j_pos Position in the j direction to start printing
 * @param canvas Pointer to the canvas to draw on
*/
static void pwnagotchi_draw_str(char* preceder, FuriString* info, const unsigned int i_pos,
                                const unsigned int j_pos, Canvas* canvas) {
    FuriString* formatTemp = furi_string_alloc_printf("%s%s", preceder, furi_string_get_cstr(info));

    canvas_set_font(canvas, PWNAGOTCHI_FONT);
    canvas_draw_str(
        canvas, j_pos, i_pos, furi_string_get_cstr(formatTemp));
    furi_string_free(formatTemp);
}

void pwnagotchi_draw_name(Pwnagotchi* pwn, Canvas* canvas) {
    FuriString* formatName = furi_string_alloc_printf("%s%s", furi_string_get_cstr(pwn->hostname), ">");

    canvas_set_font(canvas, PWNAGOTCHI_FONT);
    canvas_draw_str(canvas, PWNAGOTCHI_NAME_J, PWNAGOTCHI_NAME_I, furi_string_get_cstr(formatName));
    furi_string_free(formatName);
}

void pwnagotchi_draw_channel(Pwnagotchi* pwn, Canvas* canvas) {
    pwnagotchi_draw_str("CH ", pwn->channel, PWNAGOTCHI_CHANNEL_I, PWNAGOTCHI_CHANNEL_J, canvas);
}

void pwnagotchi_draw_aps(Pwnagotchi* pwn, Canvas* canvas) {
    pwnagotchi_draw_str("APS ", pwn->apStat, PWNAGOTCHI_APS_I, PWNAGOTCHI_APS_J, canvas);
}

void pwnagotchi_draw_uptime(Pwnagotchi* pwn, Canvas* canvas) {
    pwnagotchi_draw_str("UP ", pwn->uptime, PWNAGOTCHI_UPTIME_I, PWNAGOTCHI_UPTIME_J, canvas);
}

void pwnagotchi_draw_lines(Pwnagotchi* pwn, Canvas* canvas) {
    UNUSED(pwn);
    // Line 1
    canvas_draw_line(
        canvas,
        PWNAGOTCHI_LINE1_START_J,
        PWNAGOTCHI_LINE1_START_I,
        PWNAGOTCHI_LINE1_END_J,
        PWNAGOTCHI_LINE1_END_I);

    // Line 2
    canvas_draw_line(
        canvas,
        PWNAGOTCHI_LINE2_START_J,
        PWNAGOTCHI_LINE2_START_I,
        PWNAGOTCHI_LINE2_END_J,
        PWNAGOTCHI_LINE2_END_I);
}

void pwnagotchi_draw_handshakes(Pwnagotchi* pwn, Canvas* canvas) {
    pwnagotchi_draw_str("PWND ", pwn->handshakes, PWNAGOTCHI_HANDSHAKES_I, PWNAGOTCHI_HANDSHAKES_J, canvas);
}

void pwnagotchi_draw_friend(Pwnagotchi* pwn, Canvas* canvas) {
    UNUSED(pwn);
    UNUSED(canvas);
}

void pwnagotchi_draw_mode(Pwnagotchi* pwn, Canvas* canvas) {
    canvas_set_font(canvas, PWNAGOTCHI_FONT);
    switch(pwn->mode) {
    case PwnMode_Manual:
        canvas_draw_str(canvas, PWNAGOTCHI_MODE_MANU_J, PWNAGOTCHI_MODE_MANU_I, "MANU");
        break;
    case PwnMode_Auto:
        canvas_draw_str(canvas, PWNAGOTCHI_MODE_AUTO_J, PWNAGOTCHI_MODE_AUTO_I, "AUTO");
        break;
    case PwnMode_Ai:
        canvas_draw_str(canvas, PWNAGOTCHI_MODE_AI_J, PWNAGOTCHI_MODE_AI_I, "AI");
        break;
    }
}

void pwnagotchi_draw_message(Pwnagotchi* pwn, Canvas* canvas) {
    // [patched] rewritten: the original copied/passed a non-NUL-terminated buffer to
    // canvas_draw_str for any long word without spaces (e.g. "lookinnnnn..."), so the
    // canvas read past the allocation into heap memory -> garbled text / freeze.
    // Also maxLines underflowed (17 - 54 < 0 -> huge size_t). Now bounded + safe.
    canvas_set_font(canvas, FontSecondary);

    int fontHeight = canvas_current_font_height(canvas);
    if(fontHeight <= 0) fontHeight = 8;

    const char* message = furi_string_get_cstr(pwn->message);
    size_t messageLen = strlen(message);
    if(messageLen == 0) return;

    int charLength = canvas_string_width(canvas, "a");
    if(charLength <= 0) charLength = 1;

    size_t horizSpace = (size_t)FLIPPER_SCREEN_WIDTH - (size_t)PWNAGOTCHI_MESSAGE_J;
    size_t charSpaces = horizSpace / (size_t)charLength;
    if(charSpaces < 1) charSpaces = 1;
    if(charSpaces > 62) charSpaces = 62;

    // Vertical budget from the message row to the bottom of the screen (no underflow).
    size_t maxLines =
        ((size_t)FLIPPER_SCREEN_HEIGHT - (size_t)PWNAGOTCHI_MESSAGE_I) / (size_t)fontHeight;
    if(maxLines < 1) maxLines = 1;
    if(maxLines > 6) maxLines = 6;

    char line[64];
    size_t charIndex = 0;
    for(size_t lineNo = 0; lineNo < maxLines && charIndex < messageLen; lineNo++) {
        size_t remaining = messageLen - charIndex;
        size_t take = remaining < charSpaces ? remaining : charSpaces;

        // Avoid splitting a word: if we stopped mid-word, back up to the last space.
        if(charIndex + take < messageLen && message[charIndex + take] != ' ') {
            size_t back = take;
            while(back > 1 && message[charIndex + back - 1] != ' ') back--;
            if(back > 1 && message[charIndex + back - 1] == ' ') take = back - 1;
        }
        if(take < 1) take = 1;

        memcpy(line, message + charIndex, take);
        line[take] = '\0';

        canvas_draw_str(
            canvas,
            PWNAGOTCHI_MESSAGE_J,
            PWNAGOTCHI_MESSAGE_I + (int)(lineNo * (size_t)fontHeight),
            line);

        charIndex += take;
        while(charIndex < messageLen && message[charIndex] == ' ') charIndex++;
    }
}

void pwnagotchi_draw_all(Pwnagotchi* pwn, Canvas* canvas) {
    pwnagotchi_draw_face(pwn, canvas);
    pwnagotchi_draw_name(pwn, canvas);
    pwnagotchi_draw_channel(pwn, canvas);
    pwnagotchi_draw_aps(pwn, canvas);
    pwnagotchi_draw_uptime(pwn, canvas);
    pwnagotchi_draw_lines(pwn, canvas);
    pwnagotchi_draw_friend(pwn, canvas);
    pwnagotchi_draw_mode(pwn, canvas);
    pwnagotchi_draw_handshakes(pwn, canvas);
    pwnagotchi_draw_message(pwn, canvas);
}

void pwnagotchi_free(Pwnagotchi* pwn) {
    furi_string_free(pwn->channel);
    furi_string_free(pwn->apStat);
    furi_string_free(pwn->uptime);
    furi_string_free(pwn->hostname);
    furi_string_free(pwn->message);
    furi_string_free(pwn->handshakes);
    furi_string_free(pwn->friendStat);

    free(pwn);
}

void pwnagotchi_screen_clear(Pwnagotchi* pwn) {
    UNUSED(pwn);
}
