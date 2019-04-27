#include <iostream>
#include <fstream>
#include <math.h>
#include <time.h>
using namespace std;

const int maxHz = 256; // pixels of the image. Few seconds for 256 * 256.
const int maxVt = 256; // pixels of the image. Few seconds for 256 * 256.
const int nbComposantRGB = 3;
const int maxRGB = 256; // Each. // 65536 max.
int rgb = 1; // R = 1, G = 2, B = 3.
int imageTableauRGB_int[maxHz][maxVt][nbComposantRGB];

int pixelInit ( int x, int y, int rgb )
{
    return ( maxRGB / 2 ); // Medium gray.
}

void pixelModif ( int x, int y, int rgb, int intensity, float couleur )
{
    x = x % maxHz; // Color over the limit: go to the other side!
    y = y % maxVt; // Color over the limit: go to the other side!
    int result = imageTableauRGB_int[x][y][rgb] +  intensity * couleur; // ints tous inclus.;
    result = max ( result, 0 ); // Saturation if needed.
    result = min ( result, maxRGB ); // Saturation if needed.
    imageTableauRGB_int[x][y][rgb] = result ;
}

void brushModif ( int xPositionBrush, int yPositionBrush, int rgb, int xWidthBrush, int yWidthBrush, float douceurX = 2.0, float douceurY = 2.0 )
{
    float couleur = -1.0 + 2.0 * static_cast <float> ( rand() ) / static_cast <float> ( RAND_MAX ); // From -1 to +1 (for each RGB of course, because each brush has a constant RGB.
    // More easy: do NOT center the brush!
    int intensity = maxRGB;

    for ( int v = 0; v < yWidthBrush; v++ )
        {
        for ( int h = 0; h < xWidthBrush; h++ )
            {
            intensity =  maxRGB / ( 1 + ( pow ( ( v - ( yWidthBrush / 2 ) ) / douceurY, 2 ) + pow ( ( h - ( xWidthBrush / 2 ) ) / douceurX, 2 ) ) );
            intensity = max ( 0, intensity ); // Max to avoid problems near the sides of the brush, (maybe not needed, I don't care).
            pixelModif ( xPositionBrush + h, yPositionBrush + v, rgb, intensity, couleur ); // !
            }
        }
}

int main()
{
    srand ( time ( NULL ) ); // No need for better init.
    if ( maxRGB >= 65536 )
        {
        printf ( "\033[%dm", 31 );
        cerr << "Err: maxRGB can't be more than 65536 for PPM." << endl;
        printf ( "\033[%dm", 0 );
        cout << "End." << endl;
        }
    else
        {
        cout << "Pseudo-random tilable texture (ASCII PPM image)." << endl;
        cout << "maxHz = " << maxHz << " pixels." << endl;
        cout << "maxVt = " << maxVt << " pixels." << endl;
        cout << "maxRGB = " << maxRGB << " < 65536: ok." << endl;
        cout << "nbComposantRGB = " << nbComposantRGB << "." << endl;
        cout << "When needed, give the name without the extension, and with optional relative or absolute path." << endl;
// Create seamless trivial image (solid color), (init):
        for ( int h = 0; h < maxVt; h++ )
            {
            for ( int v = 0; v < maxHz; v++ )
                {
                for ( int rgb = 1; rgb <= nbComposantRGB; rgb++ )
                    {
                    imageTableauRGB_int[h][v][rgb] = pixelInit ( h, v, rgb );
                    }
                }
            }
// On crachote.
        int nbBrosse = maxHz * maxVt / 25; // Not needed more: too long for nothing!
        for ( int brosseNumero = 1; brosseNumero <= nbBrosse; brosseNumero++ ) // iMax inclus.
            {
            int h = rand() % maxHz + 1; // ints tous inclus.;
            int v = rand() % maxVt + 1; // ints tous inclus.;
            float  douceurX = 2.0;
            float douceurY = 2.0;
            int xBrush = 40; // Big enough for zero at the edge.
            int yBrush = 40; // Big enough for zero at the edge.
            for ( int rgb = 1; rgb <= nbComposantRGB; rgb++ )
                {
                brushModif ( h, v, rgb, xBrush, yBrush, douceurX, douceurY ); // Brush for seamless.
                }
            } // On a tout crachoté.

        clog << "Table ok." << endl;
// Convert to file:
        string name_noext = "";
        while ( name_noext == "" )
            {
            cout << "The name, please?" << endl;
            getline ( cin, name_noext );
            }
        string name_ext = name_noext + ".ppm";
        ofstream ajouteFile ( name_ext.c_str(), ios::app );
        if ( !ajouteFile )
            {
            cerr << "Impossible to open the file in order to write in it!" << endl;
            }
        else
            {
            ajouteFile << "P3" << endl; // PPM format...
            ajouteFile << maxHz << " " << maxVt << endl;
            ajouteFile << maxRGB << endl << endl;
            for ( int h = 0; h < maxVt; h++ )
                {
                for ( int v = 0; v < maxHz; v++ )
                    {
                    for ( int rgb = 1; rgb <= nbComposantRGB; rgb++ ) // iMax inclus.
                        {
                        ajouteFile << imageTableauRGB_int[h][v][rgb] << " " ;
                        }
                    ajouteFile  << endl;
                    }
                ajouteFile << endl; // For a new line.
                }
            }
        ajouteFile.close();
        clog << "File ok: " << name_ext<< endl;
        }
    return 0;
}
