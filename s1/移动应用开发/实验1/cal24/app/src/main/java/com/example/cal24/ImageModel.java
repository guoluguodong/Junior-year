package com.example.cal24;
import java.util.ArrayList;
public class ImageModel {

    public static ArrayList<Integer>  hearts=new ArrayList<Integer>();
    public static ArrayList<Integer> heartIsClicked =new ArrayList<Integer>();
    public static ArrayList<Integer>  clubs=new ArrayList<Integer>();
    public static ArrayList<Integer> clubIsClicked =new ArrayList<Integer>();
    public static ArrayList<Integer>  diamonds=new ArrayList<Integer>();
    public static ArrayList<Integer> diamondIsClicked =new ArrayList<Integer>();
    public static ArrayList<Integer>  spades=new ArrayList<Integer>();
    public static ArrayList<Integer> spadeIsClicked =new ArrayList<Integer>();
    public static ArrayList<Integer> curFourCard=new ArrayList<Integer>();
    public static void init(){
        hearts.clear();heartIsClicked.clear();clubs.clear();clubIsClicked.clear();
        diamonds.clear();diamondIsClicked.clear();spades.clear();spadeIsClicked.clear();
        curFourCard.clear();
        hearts.add(R.drawable.heart1);
        hearts.add(R.drawable.heart2);
        hearts.add(R.drawable.heart3);
        hearts.add(R.drawable.heart4);
        hearts.add(R.drawable.heart5);
        hearts.add(R.drawable.heart6);
        hearts.add(R.drawable.heart7);
        hearts.add(R.drawable.heart8);
        hearts.add(R.drawable.heart9);
        hearts.add(R.drawable.heart10);
        hearts.add(R.drawable.heart11);
        hearts.add(R.drawable.heart12);
        hearts.add(R.drawable.heart13);
        for (int i = 0; i < 13; i++) {
            heartIsClicked.add(0);
        }
        clubs.add(R.drawable.club1);
        clubs.add(R.drawable.club2);
        clubs.add(R.drawable.club3);
        clubs.add(R.drawable.club4);
        clubs.add(R.drawable.club5);
        clubs.add(R.drawable.club6);
        clubs.add(R.drawable.club7);
        clubs.add(R.drawable.club8);
        clubs.add(R.drawable.club9);
        clubs.add(R.drawable.club10);
        clubs.add(R.drawable.club11);
        clubs.add(R.drawable.club12);
        clubs.add(R.drawable.club13);
        for (int i = 0; i < 13; i++) {
            clubIsClicked.add(0);
        }
        diamonds.add(R.drawable.diamond1);
        diamonds.add(R.drawable.diamond2);
        diamonds.add(R.drawable.diamond3);
        diamonds.add(R.drawable.diamond4);
        diamonds.add(R.drawable.diamond5);
        diamonds.add(R.drawable.diamond6);
        diamonds.add(R.drawable.diamond7);
        diamonds.add(R.drawable.diamond8);
        diamonds.add(R.drawable.diamond9);
        diamonds.add(R.drawable.diamond10);
        diamonds.add(R.drawable.diamond11);
        diamonds.add(R.drawable.diamond12);
        diamonds.add(R.drawable.diamond13);
        for (int i = 0; i < 13; i++) {
            diamondIsClicked.add(0);
        }
        spades.add(R.drawable.spade1);
        spades.add(R.drawable.spade2);
        spades.add(R.drawable.spade3);
        spades.add(R.drawable.spade4);
        spades.add(R.drawable.spade5);
        spades.add(R.drawable.spade6);
        spades.add(R.drawable.spade7);
        spades.add(R.drawable.spade8);
        spades.add(R.drawable.spade9);
        spades.add(R.drawable.spade10);
        spades.add(R.drawable.spade11);
        spades.add(R.drawable.spade12);
        spades.add(R.drawable.spade13);
        for (int i = 0; i < 13; i++) {
            spadeIsClicked.add(0);
        }
        for (int i = 0; i < 4; i++) {
            curFourCard.add(0);
        }
    }
}
