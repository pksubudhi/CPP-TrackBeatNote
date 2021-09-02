// Assignment 2 21T1 COMP1511: Beats by CSE
// beats.c
//
// This program was written by PRANAV BHATNAGAR (z5555555)
// on 21-04-2021
//
// Version 1.0.0: Assignment released.

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

// Add any extra #includes your code needs here.

#include "beats.h"

// Add your own #defines here.

#define MIN_OCTAVE_VALUE 0
#define MAX_OCTAVE_VALUE 9

#define MIN_KEY_VALUE 0
#define MAX_KEY_VALUE 11

//////////////////////////////////////////////////////////////////////

// You don't have to use the provided struct track, you are free to
// make your own struct instead.
// If you use the provided struct track, you will have to add fields
// to store other information.

struct track {
    // TODO: You will have to add extra fields here in Stage 2.
    struct beat *head;
    int status; //To store if the track is stopped or playing
    struct beat *cur_beat;
};

// You don't have to use the provided struct beat, you are free to
// make your own struct instead.
// If you use the provided struct beat, you may add fields
// to it to store other information.

struct beat {
    // You may choose to add or change fields in this struct.
    struct note *notes;
    struct beat *next;
};

// You don't have to use the provided struct note, you are free to
// make your own struct instead.
// If you use the provided struct note, you add fields
// to it to store other information.

struct note {
    // You may choose to add or change fields in this struct.
    int octave;
    int key;
    struct note *next;
};

// Add any other structs you define here.

//////////////////////////////////////////////////////////////////////

// Add prototypes for any extra functions you create here.


// Return a malloced Beat with fields initialized.
Beat create_beat(void) {
    Beat new_beat = malloc(sizeof (struct beat));
    assert(new_beat != NULL);

    new_beat->next = NULL;
    new_beat->notes = NULL;

    // You do not need to change this function, unless you change
    // the implementation of struct beat.

    return new_beat;
}

// You need to implement the following functions.
// You can find descriptions of what each function should do in beats.h

//////////////////////////////////////////////////////////////////////
//                        Stage 1 Functions                         //
//////////////////////////////////////////////////////////////////////

int add_note_to_beat(Beat beat, int octave, int key)
{
	
	struct note *new_note;
	
	
	struct note *temp;
	if((octave<MIN_OCTAVE_VALUE)||(octave>MAX_OCTAVE_VALUE)) //Checking if ocate is with the given range
	{
		return INVALID_OCTAVE;
	}
	else
	{
		if((key<MIN_KEY_VALUE)||(key>MAX_KEY_VALUE)) //checking if key is within range
		{
			return INVALID_KEY;
		}
		else
		{
			
			if(beat->notes!=NULL)
			{
			//If there are notes in the beat
				temp=beat->notes;
				while(temp->next!=NULL)
				{
					temp=temp->next;
				}
				
				if(temp->octave>octave)
				{//If new note has lower octave value (Invalid Condition)
					
					return NOT_HIGHEST_NOTE;
				}
				else
				{
					if(temp->octave==octave)
					{
						if(temp->key<key)
						{ //Valid Condition (Althogh octave is same but key is higher)
						    //Allocating memory and creating a new note
							new_note=(struct note *)malloc(sizeof(struct note));
							
							new_note->key=key;
							new_note->octave=octave;
							new_note->next=NULL;
							temp->next=new_note; //Linkage
							return VALID_NOTE;
						}
						else
						{
						//If the new note has same note with either same key or smaller key
							
							return NOT_HIGHEST_NOTE;
						}
					}
					else
					{//Valid Condition
						//Allocating memory and creating a new note
						new_note=malloc(sizeof(struct note));
						
						new_note->key=key;
						new_note->octave=octave;
						new_note->next=NULL;
						
						temp->next=new_note; //linking
						return VALID_NOTE;
					}
				}
			}
			else
			{
				//If there is no note in the beat (Valid Condition)
				//Allocating memory and creating a new note
				new_note=(struct note *)malloc(sizeof(struct note));		
				new_note->key=key;
				new_note->octave=octave;
				new_note->next=NULL;
				
				beat->notes=new_note; //linking
				return VALID_NOTE;
			}
		}
	}
}


// Print the contents of a beat.
void print_beat(Beat beat) {
	struct note *prev;
	if(beat->notes==NULL){
		printf("\n");
	}
	else{
		prev=beat->notes; //Placing pointer at first note
		do{
			printf("%d %02d", prev->octave, prev->key); //Displaying until last
			if(prev->next!=NULL){
				printf(" | ");
			}
			prev=prev->next; //Moving to next note
			
		}while(prev!=NULL);
		printf("\n");
	}
    return;
}

// Count the number of notes in a beat that are in a given octave.
int count_notes_in_octave(Beat beat, int octave) {
	int cnt=0;
	struct note *temp;
	temp=beat->notes;
	if(temp==NULL){ //If no note avaialbe
		cnt=0;
	}
	else{
		while(temp!=NULL){//If there are one or more notes
			if(temp->octave==octave){ //If matched with given octave
				cnt++; //incrementing countr
			}
			temp=temp->next;
		}
	}
    return cnt;
}

//////////////////////////////////////////////////////////////////////
//                        Stage 2 Functions                         //
//////////////////////////////////////////////////////////////////////

// Return a malloced track with fields initialized.
Track create_track(void) {
    // Note: there is no fprintf in this function, as the
    // Stage 1 autotests call create_track but expect it to return NULL
    // (so this function should do nothing in Stage 1).
	Track new_track=(struct track *)malloc(sizeof(struct track));
	//assert(new_track != NULL);
	new_track->head = NULL;
    new_track->status=TRACK_STOPPED;
    new_track->cur_beat=NULL;
    return new_track;
}
void add_beat_to_track(Track track, Beat beat){
	if(track->head==NULL){ //If there is no beat in the track, adding it as first beat
		track->head=beat;
		beat->next=NULL;
	}
	else
	{
		beat->next=NULL;
		if(track->status==TRACK_STOPPED){ //If the track is stopped then adding new beat at first position
		
			beat->next=track->head;
			track->head=beat;
		}
		else{//If the track is playing then adding the new beat exactly after the selected beat
			beat->next=track->cur_beat->next;
			track->cur_beat->next=beat;
		}
	}
}

// Set a track's current beat to the next beat.
int select_next_beat(Track track) 
{
	
	if(track->status==TRACK_STOPPED)
	{
		//If the track is Stopped
		if(track->head!=NULL)
		{//Starts playing if there esits some beat
		
			track->status=TRACK_PLAYING;
			track->cur_beat=track->head;
			return TRACK_PLAYING;
		}
		else
		{//If there is no no beat at all
			track->status=TRACK_STOPPED;
			track->cur_beat=NULL;
			return TRACK_STOPPED;
		}
	}
	else
	{
		if(track->cur_beat->next==NULL)
		{	//If the currently selected beat is the last one
			track->cur_beat=NULL;
			track->status=TRACK_STOPPED;
			return TRACK_STOPPED;
		}
		else
		{
			track->cur_beat=track->cur_beat->next; //If any other 
			track->status=TRACK_PLAYING;
			return TRACK_PLAYING;
		}
	}
}

// Print the contents of a track.
void print_track(Track track) {
	struct note *temp_note=NULL;
    if(track->head==NULL){//If beats not avaialable
	}
	else{
		int i=1; //counter for beat index
		Beat temp_beat;
		temp_beat=track->head;
		while(temp_beat!=NULL){	
			if(temp_beat==track->cur_beat){ //Placing an indicator at the currnt note
				printf(">");
			}
			else{
				printf(" "); //If not the current beat in play
			}
			printf("[%d] ", i); //Printing track index 1 to n
			temp_note=temp_beat->notes;
			while(temp_note!=NULL){
				printf("%d %02d", temp_note->octave, temp_note->key); //Showing notes (octave and key) in the beat
				if(temp_note->next!=NULL){
					printf(" | ");
				}
				temp_note=temp_note->next; //Moving forward note to note
			}
			printf("\n");
			temp_beat=temp_beat->next; //Moving beat to beat
			i++; //Incrementing counter
		}
	}
    return;
}

// Count beats after the current beat in a track.
int count_beats_left_in_track(Track track) {
    int cnt=0;
    Beat cur_beat;
    if(track->status==TRACK_STOPPED){ //If the track is stopped then it should count all beats available in the track
    	cur_beat=track->head; 	
	}
	else{
		cur_beat=track->cur_beat->next; //If the track is playing then it should count all beats next to current one
	}
	while(cur_beat!=NULL){
		cur_beat=cur_beat->next; //Moving forward
		cnt++; //Incrementing counter
	}
    return cnt;
}

//////////////////////////////////////////////////////////////////////
//                        Stage 3 Functions                         //
//////////////////////////////////////////////////////////////////////

void free_beat(Beat beat)
{
	struct note *temp, *prev;
	if(beat==NULL)
	{
		
	}
	else
	{
		if(beat->notes==NULL) //If there is no note
		{
			free(beat);
		}
		else
		{
			if(beat->notes->next==NULL) //If there only one note
			{
				free(beat->notes);
				free(beat);
			}
			else //If there are many notes
			{
				temp=beat->notes;	//Placing temp pointer on last note
				while(temp->next!=NULL)
				{
					temp=temp->next;
				}
				while(temp!=beat->notes)//Moving in backward direction while refrshing one by one
				{
					prev=beat->notes;
					while(prev->next!=temp)
					{
						prev=prev->next;
					}
					prev->next=NULL;
					//deallocating memory to the note
					free(temp);
					temp=prev;
				}
				free(temp);
				free(beat);
			}
		}
	}
	return;
}
void free_track(Track track)
{
	struct beat *temp, *prev;
	if(track==NULL) //If track is NULL
	{
		
	}
	else
	{
		if(track->head==NULL) //If track is there but no beat
		{
			free(track);
		}
		else
		{
			if(track->head->next==NULL)// If there is only one beat
			{
				free_beat(track->head); //clearingmemoryassociate dwith the beat
				free(track);
			}
			else //If there are many beats
			{
				temp=track->head; //Placing temp pointer at last
				while(temp->next!=NULL)
				{
					temp=temp->next;
				}
				while(temp!=track->head) //Move backward direction by clearing beat by beat
				{
					prev=track->head;
					while(prev->next!=temp)
					{
						prev=prev->next;
					}
					prev->next=NULL;
					free_beat(temp); //clearingmemoryassociate dwith the beat
					temp=prev;
				}
				free_beat(temp); //clearingmemoryassociate dwith the beat
				free(track);
			}
		}
	}
	return;
}
int remove_selected_beat(Track track)
{
	Beat temp;
	if(track->status==TRACK_STOPPED)
	{
		return TRACK_STOPPED;
	}
	else
	{
		if(track->head->next==NULL) //If the selected beat is the first beat and there is only one beat
		{
			free_beat(track->head);
			track->head=NULL;
			track->cur_beat=NULL;
			track->status=TRACK_STOPPED;
			return TRACK_STOPPED;
		}
		else
		{
			if(track->cur_beat==track->head) //If the selected the first beat and there other beads after that
			{
				temp=track->head;
				track->head=temp->next;
				track->cur_beat=temp->next;
				track->status=TRACK_PLAYING;
				free_beat(temp);
				//free(temp);
				return TRACK_PLAYING;
			}
			else
			{
				if(track->cur_beat->next==NULL) //If the selected beat is the last one
				{
					temp=track->head;
					while(temp->next!=track->cur_beat)
					{
						temp=temp->next;
					}
					temp->next=NULL;
					free_beat(track->cur_beat);
					//free(track->cur_beat);
					track->cur_beat=NULL;
					track->status=TRACK_STOPPED;
					return TRACK_STOPPED;
				}
				else
				{
					temp=track->head;
					while(temp->next!=track->cur_beat)
					{
						temp=temp->next;
					}
					temp->next=track->cur_beat->next;
					free_beat(track->cur_beat);
					//free(track->cur_beat);
					track->cur_beat=temp->next;
					track->status=TRACK_PLAYING;
					return TRACK_PLAYING;
					
				}
			}
			
		}
	}
}

