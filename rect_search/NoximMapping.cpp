//Tcarenko
void NoximMapping::map_build_rect_pan(NoximInvokedApplication *invApp)
{
	NoximApplication *app = invApp ->Application;
	//initial rectangle
	NoximRectangle rectInit;
	//rectangle for panning
	NoximRectangle rectPan;
	if(invApp ->invID == 15)
		rectInit.ldCorner.X = 0;

	rectInit.ldCorner = invApp ->fnID;
	rectInit.ruCorner = invApp ->fnID;

	//build the initial large rectangle
	for(int i = 0; i < app ->appSize; i++)
	{
		if(invApp ->AllocatedPE[i].X > rectInit.ruCorner.X)
			rectInit.ruCorner.X = invApp ->AllocatedPE[i].X;
		if(invApp ->AllocatedPE[i].Y > rectInit.ruCorner.Y)
			rectInit.ruCorner.Y = invApp ->AllocatedPE[i].Y;
		if(invApp ->AllocatedPE[i].X < rectInit.ldCorner.X)
			rectInit.ldCorner.X = invApp ->AllocatedPE[i].X;
		if(invApp ->AllocatedPE[i].Y < rectInit.ldCorner.Y)
			rectInit.ldCorner.Y = invApp ->AllocatedPE[i].Y;
	}
	//get the initial rectangle's dimensions
	int height = rectInit.height();
	int length = rectInit.width();

	//split the initial rectangle into 4 rectangles and find the dimensions of the largest rectangle
	int heightPan = height%2 == 0? (height/2) : (height-1)/2;	//height of the largest part
	int widthPan = length%2 == 0? (length/2) : (length-1)/2;	//width of the largest part

	//int heightPan = (height/2)+1;	//height of the largest part
	//int widthPan = (length/2)+1;	//width of the largest part


	//set the coordinates of the found smallest part of the rectangle. place it to the left down corner
	rectPan.ldCorner.X = rectInit.ldCorner.X; //left down corner
	rectPan.ldCorner.Y = rectInit.ldCorner.Y;

	rectPan.ruCorner.X = rectPan.ldCorner.X + widthPan - 1;	//right up corner
	rectPan.ruCorner.Y = rectPan.ldCorner.Y + heightPan - 1;

	unsigned int minPen = 0;	//minimum penalty
	unsigned char ldX = 0, ldY = 0;		//left down x,y coordinate 
	int included = 0, includedBefore = 0;
	unsigned int wMax = rectInit.width()-rectPan.width();
	unsigned int hMax = rectInit.height()-rectPan.height();
	//panning. find the position of the panning rect where the penalty is the least
	for(int i = 0; i <=wMax; i++)	{
		for(int j = 0; j <= hMax; j++)	{
			//calculate the penalty for the current position of the pan rect
			if(i==0 && j==0) {
				//initialization
				minPen = rectCountPenalty(rectPan, invApp, included);
				ldX = rectPan.ldCorner.X;
				ldY = rectPan.ldCorner.Y;
			}
			else {
				//if the current position is better, save it
				if(minPen > rectCountPenalty(rectPan, invApp, included)) {
					//new minimum is found. rewrite the values
					minPen = rectCountPenalty(rectPan, invApp, included);
					ldX = rectPan.ldCorner.X;
					ldY = rectPan.ldCorner.Y;
				}
			}
			//move pan rect up
			rectPan.ldCorner.Y++;
			rectPan.ruCorner.Y++;
		}
		//move pan rect right
		rectPan.ldCorner.X++;
		rectPan.ruCorner.X++;
		//return Y coord to initial position
		rectPan.ldCorner.Y = rectInit.ldCorner.Y;
		rectPan.ruCorner.Y =  rectPan.ldCorner.Y + heightPan - 1;
	}
	
	//restore best pan rect position
	rectPan.ldCorner.X = ldX;
	rectPan.ldCorner.Y = ldY;

	rectPan.ruCorner.X = rectPan.ldCorner.X + widthPan - 1;	//right up
	rectPan.ruCorner.Y = rectPan.ldCorner.Y + heightPan - 1;
	
	//now the position is known. do shrinking-expanding
	int penArr[numberOfDir];
	int min;
	//vector of actions to do
	vector<int> actions;
	int actionArr[numberOfDir];
	vector<pair<int, bool>> penVec;
	bool mod = false;
	rectCountPenalty(rectPan, invApp, includedBefore);
	do{
		//do 8 actions
		for(int i = 0; i < numberOfDir; i++){
			//do action
			mod = shrink_expand_rect(&rectPan, (direction)i);
			//check the penalty and included nodes after the modification
			int curr_penalty = rectCountPenalty(rectPan, invApp, included);
			//check included nodes
			if(included == includedBefore){
				//action
				if(mod) {
					//if the action is shrinking. if the empty line or column was cut out -> prioritize action
					if(i%2 != 0)penVec.push_back(make_pair(curr_penalty, true));
					else penVec.push_back(make_pair(curr_penalty, false));
				}
				else penVec.push_back(make_pair(-1, false));
			}else{
				//just a simple action
				if(mod) penVec.push_back(make_pair(curr_penalty, false));
				else penVec.push_back(make_pair(-1, false));
			}
			//rollback
			if(mod) shrink_expand_rect_rollback(&rectPan, (direction)i);
		}
		//the array of penalties is filled. find the minimum penalty and do the action step if this step leads to decreasing the overall penalty
		build_actions(penVec, &actions);
		//current penalty before the action
		int currPen = rectCountPenalty(rectPan, invApp, included);
		//check if the found actions the penalty will be smaller
		if(penVec[actions[0]].first >= currPen){
			//new penalty is bigger
			break;
		}else{
			//new penalty is smaller. apply all the actions
			int actionsSize = actions.size();
			for(int i = 0; i < actionsSize; i++){
				shrink_expand_rect(&rectPan, (direction)actions[i]);
				//recalculate the number of rightly included nodes
				rectCountPenalty(rectPan, invApp, includedBefore);
			}
		}
		penVec.clear();
		actions.clear();
	}while(true);
	//flush the vector
	actions.clear();
	//save pan rect
	//invApp ->rect = rectPan;
	////save statistics
	//invApp ->penalty = rectCountPenalty(rectPan, invApp, included);
	//invApp ->penPercent = ((double)invApp ->penalty)/min((invApp ->rect.area()),invApp ->Application ->appSize) * 100;
}
//Tcarenko. the function first finds the largest continuous rectangle type area without empty nodes, then it is expanded. 
void NoximMapping::map_build_rect_largest_area_first(NoximInvokedApplication*  invApp)
{
	NoximApplication *app = invApp ->Application;
	//initial rectangle
	NoximRectangle rectInit;
	//get the coverage rect
	rectInit = find_cover_rect(invApp);
	//number of correctly included nodes
	int included = 0;
	//run
	find_largest_cont_rect(invApp, rectInit);
	vector<pair<NoximRectangle, int>> rectPen;
	//vector<pair<NoximRectangle, int>> rectPenFin;
	vector<NoximRectangle> bestSolVecBuf = aWorm.bestSolVec;

	for(int currBestSol = 0; currBestSol < bestSolVecBuf.size(); currBestSol++){
		
		bool modified = false;
		bool hasImprovement = false;
		for(int i = 0; i < numberOfDir; i++){
			//do each action while it is possible to do it
			do{
				int pen = rectCountPenalty(bestSolVecBuf[currBestSol], invApp, included);
				pair<NoximRectangle, int> pairToSave;
				//save
				pairToSave = make_pair(bestSolVecBuf[currBestSol], pen);
				rectPen.push_back(pairToSave);
				//modify
				modified = shrink_expand_rect(&bestSolVecBuf[currBestSol], (direction)i);
			}while(modified);
			//find the best among them
			int bestInd = getMinimumInd(rectPen);
			//save rect
			NoximRectangle bestRectInDir = rectPen[bestInd].first;
			//clear the vector of pairs
			//rectPen.clear();
			//do anti action
			int antiAction = getAntiAction(i);
			do{
				int pen = rectCountPenalty(bestRectInDir, invApp, included);
				pair<NoximRectangle, int> pairToSave;
				//save
				pairToSave = make_pair(bestRectInDir, pen);
				rectPen.push_back(pairToSave);
				//modify
				modified = shrink_expand_rect(&bestRectInDir, (direction)antiAction);
			}while(modified);
			//find the best solution
			bestInd = getMinimumInd(rectPen);
			//save the best solution
			//rectPenFin.push_back(rectPen[bestInd]);
			bestSolVecBuf = aWorm.bestSolVec;
		}
	}

	//now we have a vector of pairs of possible solutions. search for a area with the smallest penalty
	int minInd = getMinimumInd(rectPen);
	//the answer is rectPen[minInd].first
	invApp ->rectVec.clear();
	invApp ->rectVec.push_back(rectPen[minInd].first);
	//save statistics
	invApp ->penalty = rectCountPenalty(invApp ->rectVec[0], invApp, included);
	invApp ->penPercent = ((double)invApp ->penalty)/min((invApp ->rectVec[0].area()),invApp ->Application ->appSize) * 100;

}

void NoximMapping::map_build_rect_several_rects(NoximInvokedApplication* invApp){
	//initial rect
	NoximRectangle rectInit = find_cover_rect(invApp);
	do{
		//get the list of the rectangles
		find_largest_cont_rects(invApp, rectInit);
		//sort the list
		sort_largest_rects();
		//only the best solution should be left in the vector
		if(!aWorm.largestRects.empty()){
			aWorm.largestRects.erase(aWorm.largestRects.begin(), aWorm.largestRects.begin()+aWorm.largestRects.size()-1);
			//here the largest rect should be expanded and then pushed back to the omitted vector
			expand_rect_min_penalty(invApp, &aWorm.largestRects[0]);
		}else break;
		//the application is modeled with the rectangle(s) from "omitted vector".analyze the solution. if needed, add more rects
		if(!aWorm.omitRects.empty()){
			if(aWorm.largestRects[0].area() < NoximGlobalParams::smallestSizeOfRect || calc_number_of_excluded_nodes(invApp) < NoximGlobalParams::allowedNumOfExcludedNodes) break;
			else aWorm.omitRects.push_back(aWorm.largestRects[0]);
		}else aWorm.omitRects.push_back(aWorm.largestRects[0]);
	}while(true);
	invApp ->rectVec = aWorm.omitRects;
	int area;
	invApp ->penalty = calculate_penalty_rects(invApp, area);
	invApp ->penPercent = ((double)invApp ->penalty)/min(area,invApp ->Application ->appSize) * 100;
	aWorm.omitRects.clear();
}
//Tcarenko. calculate penalty
int NoximMapping::calculate_penalty_rects(NoximInvokedApplication* invApp, int& area){
	int inclSum = 0, exclSum = 0, areaSum = 0;
	for(int i = 0; i < invApp ->rectVec.size(); i++){
		int included = 0;
		rectCountPenalty(invApp ->rectVec[i], invApp, included);
		inclSum += included;
		areaSum += invApp ->rectVec[i].area();
	}
	exclSum = invApp ->Application ->appSize - inclSum;
	area = areaSum;
	return(exclSum + areaSum - inclSum);
}
//Tcarenko. calculate the number of excluded nodes
int NoximMapping::calc_number_of_excluded_nodes(NoximInvokedApplication* invApp){
	int vecSize = aWorm.omitRects.size();
	int exclNodes = 0;
	if(vecSize == 0) return 0;
	for(int i = 0; i < vecSize; i++){
		int included = 0;
		rectCountPenalty(aWorm.omitRects[i], invApp, included);
		exclNodes += invApp ->Application ->appSize - included;
	}
	return exclNodes;
}
//Tcarenko. expand rect considering penalty
void NoximMapping::expand_rect_min_penalty(NoximInvokedApplication* invApp, NoximRectangle* rect){
	bool modified = false;
	bool improved = false;
	int included = 0;
	do{
		improved = false;
		for(int i = 0; i < numberOfDir; i++){
			int penBefore = rectCountPenalty(*rect, invApp, included);
			modified = shrink_expand_rect(rect, (direction) i);
			if(modified){
				int penAfter = rectCountPenalty(*rect, invApp, included);
				if(penAfter >= penBefore || has_intersections_with_omitted(*rect)){
					shrink_expand_rect_rollback(rect, (direction)i);
				}else improved = true;
			}
		}
	}while(improved);
}
//Tcarenko. the function finds the largest rectangles
void NoximMapping::find_largest_cont_rect(NoximInvokedApplication* invApp, NoximRectangle rectInit){
	//get the initial rectangle's dimensions
	int height = rectInit.height();
	int width = rectInit.width();
	//the worm starts to move from left down corner of the initial rect
	//the worm chooses only dir and up expansions
	aWorm.head.X = rectInit.ldCorner.X;
	aWorm.head.Y = rectInit.ldCorner.Y;

	aWorm.bestSol.ldCorner.X = aWorm.head.X;
	aWorm.bestSol.ldCorner.Y = aWorm.head.Y;
	aWorm.bestSol.ruCorner.X = aWorm.head.X;
	aWorm.bestSol.ruCorner.Y = aWorm.head.Y;
	//clear the best sol vector
	aWorm.bestSolVec.clear();
	//choose the initial direction -> to the right
	aWorm.dir = rightE;
	int included = 0;
	//run
	for(int i=0; i<height; i++){
		for(int j=0; j<width; j++){
			//assess current head position
			aWorm.currSol.ldCorner = aWorm.head;
			aWorm.currSol.ruCorner = aWorm.head;
			rectCountPenalty(aWorm.currSol, invApp, included);
			if(included != 0){
				//the node is not empty
				bool dirHasEmpty = true, upHasEmpty = true;
				//dir first
				do{
					//expand the rect in the dir direction and assess
					bool modified = false;
					modified = shrink_expand_rect(&aWorm.currSol, aWorm.dir);
					rectCountPenalty(aWorm.currSol, invApp, included);
					if(included == aWorm.currSol.area() && modified) {
						dirHasEmpty = false;
						if(aWorm.bestSolVec.empty()) aWorm.bestSolVec.push_back(aWorm.currSol);
						else{
							//if the solution is the best, save it to the best solutions vector
							if(aWorm.currSol.area() > aWorm.bestSolVec[0].area()) {
								//the new best solution is found -> bestSolVec should be cleared
								aWorm.bestSolVec.clear();
								aWorm.bestSolVec.push_back(aWorm.currSol);
							}else if(aWorm.currSol.area() == aWorm.bestSolVec[0].area()){
								//another solution which is equal to the best one found -> save it
								aWorm.bestSolVec.push_back(aWorm.currSol);
							}
						}
					}
					else if(modified) shrink_expand_rect_rollback(&aWorm.currSol, aWorm.dir);

					//expand the rect in the up direction and assess
					modified = shrink_expand_rect(&aWorm.currSol, upE);
					rectCountPenalty(aWorm.currSol, invApp, included);
					if(included == aWorm.currSol.area() && modified) {
						upHasEmpty = false;
						if(aWorm.bestSolVec.empty()) aWorm.bestSolVec.push_back(aWorm.currSol);
						else{
							//if the solution is the best, save it to the best solution vector
							if(aWorm.currSol.area() > aWorm.bestSolVec[0].area()) {
								//the new best solution is found -> bestSolVec should be cleared
								aWorm.bestSolVec.clear();
								aWorm.bestSolVec.push_back(aWorm.currSol);
							}else if(aWorm.currSol.area() == aWorm.bestSolVec[0].area()){
								//another solution which is equal to the best one found -> save it
								aWorm.bestSolVec.push_back(aWorm.currSol);
							}
						}
					}
					else if(modified) shrink_expand_rect_rollback(&aWorm.currSol, upE);
					if(dirHasEmpty && upHasEmpty) break;
					dirHasEmpty = true; upHasEmpty = true;
				}
				while(true);
				aWorm.currSol.ldCorner = aWorm.head;
				aWorm.currSol.ruCorner = aWorm.head;
				//up first
				do{
					//expand the rect in the dir direction and assess
					bool modified = false;
					//expand the rect in the up direction and assess
					modified = shrink_expand_rect(&aWorm.currSol, upE);
					rectCountPenalty(aWorm.currSol, invApp, included);
					if(included == aWorm.currSol.area() && modified) {
						upHasEmpty = false;
						if(aWorm.bestSolVec.empty()) aWorm.bestSolVec.push_back(aWorm.currSol);
						else{
							//if the solution is the best, save it to the best solution vector
							if(aWorm.currSol.area() > aWorm.bestSolVec[0].area()) {
								//the new best solution is found -> bestSolVec should be cleared
								aWorm.bestSolVec.clear();
								aWorm.bestSolVec.push_back(aWorm.currSol);
							}else if(aWorm.currSol.area() == aWorm.bestSolVec[0].area()){
								//another solution which is equal to the best one found -> save it
								aWorm.bestSolVec.push_back(aWorm.currSol);
							}
						}
					}
					else if(modified) shrink_expand_rect_rollback(&aWorm.currSol, upE);

					modified = shrink_expand_rect(&aWorm.currSol, aWorm.dir);
					rectCountPenalty(aWorm.currSol, invApp, included);
					if(included == aWorm.currSol.area() && modified) {
						dirHasEmpty = false;
						if(aWorm.bestSolVec.empty()) aWorm.bestSolVec.push_back(aWorm.currSol);
						else{
							//if the solution is the best, save it to the best solution vector
							if(aWorm.currSol.area() > aWorm.bestSolVec[0].area()) {
								//the new best solution is found -> bestSolVec should be cleared
								aWorm.bestSolVec.clear();
								aWorm.bestSolVec.push_back(aWorm.currSol);
							}else if(aWorm.currSol.area() == aWorm.bestSolVec[0].area()){
								//another solution which is equal to the best one found -> save it
								aWorm.bestSolVec.push_back(aWorm.currSol);
							}
						}
					}
					else if(modified) shrink_expand_rect_rollback(&aWorm.currSol, aWorm.dir);

					if(dirHasEmpty && upHasEmpty) break;
					dirHasEmpty = true; upHasEmpty = true;
				}
				while(true);
			}
			//move
			if(aWorm.dir == rightE)aWorm.head.X++;
			else aWorm.head.X--;
		}
		//change the direction
		if(aWorm.dir == rightE) aWorm.dir = leftE;
		else aWorm.dir = rightE;

		if(aWorm.dir == rightE) aWorm.head.X = rectInit.ldCorner.X;
		else aWorm.head.X = rectInit.ruCorner.X;
		
		aWorm.head.Y++;
	}
}
//Tcarenko. finds rectangles
void NoximMapping::find_largest_cont_rects(NoximInvokedApplication* invApp, NoximRectangle rectInit){
	//the worm starts to move from left down corner of the initial rect
	//the worm chooses only dir and up expansions
	aWorm.head = rectInit.ldCorner;
	//choose the initial direction -> to the right
	aWorm.dir = rightE;
	//if(!get_next_head_position(rectInit, false)) return;
	//clear the best sol vector
	aWorm.largestRects.clear();
	
	int included = 0;
	bool canMove = false;
	//run
	//do{//X
		do{
			bool hasNewSolution = false;
			//assess current head position
			aWorm.initSol.ldCorner = aWorm.head;
			aWorm.initSol.ruCorner = aWorm.head;
			rectCountPenalty(aWorm.initSol, invApp, included);
			if(included != 0){
				//the node is not empty
				bool modified;
				//expand the rect in the dir direction as far as possible
				do{
					//it can be expanded if it doesnt intersect with areas which should be omitted
					modified = shrink_expand_rect(&aWorm.initSol, aWorm.dir);
					//check if it intersects
					if(modified && has_intersections_with_omitted(aWorm.initSol)) {
						//it does intersect -> break;
						shrink_expand_rect_rollback(&aWorm.initSol, aWorm.dir);
						break;
					}

					rectCountPenalty(aWorm.initSol, invApp, included);
					if(included == aWorm.initSol.area() && modified) ;
					else if(modified) {
						shrink_expand_rect_rollback(&aWorm.initSol, aWorm.dir);
						break;
					}else break;
				}while(true);
				aWorm.currSol = aWorm.initSol;
				aWorm.bestSol = aWorm.initSol;
				//we have the largest horizontal stripe for the current head position. try to expand it up
				do{
					//expand up
					modified = shrink_expand_rect(&aWorm.currSol, upE);
					rectCountPenalty(aWorm.currSol, invApp, included);
					if(included == aWorm.currSol.area() && modified && !has_intersections_with_omitted(aWorm.currSol)){
						//check if it is better solution
						if(aWorm.currSol.area() > aWorm.bestSol.area()) aWorm.bestSol = aWorm.currSol;
					}else{
						//shrink init sol
						modified = shrink_expand_rect(&aWorm.initSol, (direction)(aWorm.dir+1));
						//if impossible to shrink -> the area consists of a single node
						if(modified) aWorm.currSol = aWorm.initSol;
						else break;
					}
				}while(true);
				aWorm.largestRects.push_back(aWorm.bestSol);
				hasNewSolution = true;
			}
			//move
			canMove = get_next_head_position(rectInit, false);
			hasNewSolution = false;
		}while(canMove);
}
//Tcarenko. Sorts found rectangles
void NoximMapping::sort_largest_rects(){
	NoximRectangle rectBuf;
	for(int i = aWorm.largestRects.size()-1; i >= 0; i--){
		unsigned maxInd = i;
		unsigned maxArea = aWorm.largestRects[maxInd].area();
		for(int j = 0; j < i; j++){//find max
			if(aWorm.largestRects[j].area() > maxArea){
				maxInd = j;
				maxArea = aWorm.largestRects[j].area();
			}
		}
		//replace
		rectBuf = aWorm.largestRects[i];
		aWorm.largestRects[i] = aWorm.largestRects[maxInd];
		aWorm.largestRects[maxInd] = rectBuf;
	}
}
//Tcarenko. returns true if the rect has intersections with previous rects
bool NoximMapping::has_intersections_with_omitted(NoximRectangle rect){
	NoximRectangle rectRes;
	for(int i = 0; i < aWorm.omitRects.size(); i++){
		if(getIntersection(rect, aWorm.omitRects[i], &rectRes)) return true;
		else if(getIntersection(aWorm.omitRects[i], rect, &rectRes)) return true;
	}
	return false;
}
//Tcarenko. Merge and split the rects
void NoximMapping::merge_rects(void){
	//start from the largest one. merging first
	int size = aWorm.largestRects.size();
	for(int i = size-1; i >= 0; i--){
		for(int j = i-1; j>=0; j--){
			NoximRectangle interRect;
			getIntersection(aWorm.largestRects[i], aWorm.largestRects[j], &interRect);
			//compare only with the j-th rect. since i-th rect is equal or larger than j-th
			if(interRect == aWorm.largestRects[j]){
				//means that j-th rect is fully in i-th -> erase it from the vector
				aWorm.largestRects.erase(aWorm.largestRects.begin()+j);
				i--;
			}
		}
	}
}

//Tcarenko. Worm should leap over squares already found
bool NoximMapping::get_next_head_position(NoximRectangle rectInit, bool hasNewSolution){
	//leap in the current direction. leap on the width of the found rect
	int width = aWorm.bestSol.width();
	if(hasNewSolution){
		if(aWorm.dir == rightE)	aWorm.head.X += (width);
		else aWorm.head.X -= (width);
	}else{
		if(aWorm.dir == rightE)	aWorm.head.X++;
		else aWorm.head.X--;
	}

	if(!(aWorm.head.X >= rectInit.ldCorner.X && aWorm.head.X <= rectInit.ruCorner.X)){
		//out of init square
		if(aWorm.dir == rightE) {
			aWorm.dir = leftE;
			aWorm.head.X = rectInit.ruCorner.X;
		}else {
			aWorm.dir = rightE;
			aWorm.head.X = rectInit.ldCorner.X;
		}
		aWorm.head.Y++;
		if(!(aWorm.head.Y >= rectInit.ldCorner.Y && aWorm.head.Y <= rectInit.ruCorner.Y)) return false;
	}


	//leap over the previous solutions
	for(int i = 0; i < aWorm.omitRects.size(); i++){
		//if the head inside the area which should be omitted
		do{
			if(aWorm.omitRects[i].include(aWorm.head)){
				width = aWorm.omitRects[i].width();

				if(aWorm.dir == rightE)	aWorm.head.X += (width);
				else aWorm.head.X -= (width);

				if(!(aWorm.head.X >= rectInit.ldCorner.X && aWorm.head.X <= rectInit.ruCorner.X)){
					//out of init square
					if(aWorm.dir == rightE) {
						aWorm.dir = leftE;
						aWorm.head.X = rectInit.ruCorner.X;
					}else {
						aWorm.dir = rightE;
						aWorm.head.X = rectInit.ldCorner.X;
					}
					aWorm.head.Y++;
					if(!(aWorm.head.Y >= rectInit.ldCorner.Y && aWorm.head.Y <= rectInit.ruCorner.Y)) return false;
				}
			}else break;
		}while(true);
	}
	return true;
}

//Tcarenko. the function finds for a cover rectangle
NoximRectangle NoximMapping::find_cover_rect(NoximInvokedApplication* invApp){
	
	NoximRectangle rectInit;

	rectInit.ldCorner = invApp ->fnID;
	rectInit.ruCorner = invApp ->fnID;

	//build the initial large rectangle
	for(int i = 0; i < invApp ->Application ->appSize; i++)
	{
		if(invApp ->AllocatedPE[i].X > rectInit.ruCorner.X)
			rectInit.ruCorner.X = invApp ->AllocatedPE[i].X;
		if(invApp ->AllocatedPE[i].Y > rectInit.ruCorner.Y)
			rectInit.ruCorner.Y = invApp ->AllocatedPE[i].Y;
		if(invApp ->AllocatedPE[i].X < rectInit.ldCorner.X)
			rectInit.ldCorner.X = invApp ->AllocatedPE[i].X;
		if(invApp ->AllocatedPE[i].Y < rectInit.ldCorner.Y)
			rectInit.ldCorner.Y = invApp ->AllocatedPE[i].Y;
	}

	return rectInit;
}
//Tcarenko. returns the index of the minimum
int NoximMapping::getMinimumInd(vector<pair<NoximRectangle, int>> vec){
	int minInd = 0;
	NoximRectangle rect = vec[minInd].first;
	int min = vec[minInd].second;
	for(int i = 0; i < vec.size(); i++){
		if(vec[i].second < min){
			min = vec[i].second;
			rect = vec[i].first;
			minInd = i;
		}else if(vec[i].second == min && vec[i].first.area() > rect.area()){
			min = vec[i].second;
			rect = vec[i].first;
			minInd = i;
		}
	}
	return minInd;
}
//Tcarenko. decypher
int NoximMapping::getAntiAction(int action){
	switch ((direction)action){
		case leftE		: return rightE;
		case leftS		: return rightS;
		case rightE		: return leftE;
		case rightS		: return leftS;
		case	upE		: return downE;
		case	upS		: return downS;
		case  downE		: return upE;
		case  downS		: return upS;
		case leftUpE	: return rightDownE;		//left-up expansion
		case leftDownE	: return rightUpE;
		case rightUpE	: return leftDownE;
		case rightDownE : return leftUpE;
	}
}

//Tcarenko. returns true if 2 rects have an intersection
bool NoximMapping::getIntersection(NoximRectangle rectA, NoximRectangle rectB, NoximRectangle* res){
	NoximRectangle interRect;
	//A in B
	if((rectA.ldCorner.X > rectB.ldCorner.X && rectA.ldCorner.X < rectB.ruCorner.X) &&
	   (rectA.ldCorner.Y > rectB.ldCorner.Y && rectA.ldCorner.Y < rectB.ruCorner.Y) &&
	   (rectA.ruCorner.X > rectB.ldCorner.X && rectA.ruCorner.X < rectB.ruCorner.X) &&
	   (rectA.ruCorner.Y > rectB.ldCorner.Y && rectA.ruCorner.Y < rectB.ruCorner.Y)){
		*res = rectA;
		return true;
	}else
	//B in A
	if((rectB.ldCorner.X > rectA.ldCorner.X && rectB.ldCorner.X < rectA.ruCorner.X) &&
	   (rectB.ldCorner.Y > rectA.ldCorner.Y && rectB.ldCorner.Y < rectA.ruCorner.Y) &&
	   (rectB.ruCorner.X > rectA.ldCorner.X && rectB.ruCorner.X < rectA.ruCorner.X) &&
	   (rectB.ruCorner.Y > rectA.ldCorner.Y && rectB.ruCorner.Y < rectA.ruCorner.Y)){
		*res = rectB;
		return true;
	}
	//if any of 4 vertices of any rect is inside the other rect
	if((rectA.ldCorner.X >= rectB.ldCorner.X && rectA.ldCorner.X <= rectB.ruCorner.X) &&	//left down corner in
	   (rectA.ldCorner.Y >= rectB.ldCorner.Y && rectA.ldCorner.Y <= rectB.ruCorner.Y)){
		interRect.ldCorner = rectA.ldCorner;
		interRect.ruCorner = rectB.ruCorner;
		*res = interRect;
		return true;
	}else
	   
	//right down
	if((rectA.ruCorner.X >= rectB.ldCorner.X && rectA.ruCorner.X <= rectB.ruCorner.X) &&
	   (rectA.ldCorner.Y >= rectB.ldCorner.Y && rectA.ldCorner.Y <= rectB.ruCorner.Y)){
		interRect.ldCorner.X = rectB.ldCorner.X;
		interRect.ldCorner.Y = rectA.ldCorner.Y;
		interRect.ruCorner.X = rectA.ruCorner.X;
		interRect.ruCorner.Y = rectB.ruCorner.Y;
		*res = interRect;
		return true;
	}else
	//right up
	if((rectA.ruCorner.X >= rectB.ldCorner.X && rectA.ruCorner.X <= rectB.ruCorner.X) &&
	   (rectA.ruCorner.Y >= rectB.ldCorner.Y && rectA.ruCorner.Y <= rectB.ruCorner.Y)){
	    interRect.ldCorner = rectB.ldCorner;
		interRect.ruCorner = rectA.ruCorner;
		*res = interRect;
		return true;
	}else
	//left up
	if((rectA.ldCorner.X >= rectB.ldCorner.X && rectA.ldCorner.X <= rectB.ruCorner.X) &&
	   (rectA.ruCorner.Y >= rectB.ldCorner.Y && rectA.ruCorner.Y <= rectB.ruCorner.Y)){
	    interRect.ldCorner.X = rectA.ldCorner.X;
		interRect.ldCorner.Y = rectB.ldCorner.Y;
		interRect.ruCorner.X = rectB.ruCorner.X;
		interRect.ruCorner.Y = rectA.ruCorner.Y;
		*res = interRect;
		return true;
	}else return false;
}
//Tcarenko. Builds the action set
void NoximMapping::build_actions(vector<pair<int, bool>> &penVec, vector<int>* actions)
{
	int min = -1;
	int minInd = 0;
	bool prioActions = false;
	//first check if there are prioritized actions
	for(int i = 0; i < penVec.size(); i++){
		if(penVec[i].second){
			prioActions = true;
			actions ->push_back(i);
		}
	}
	if(!prioActions){
		//no prioritized actions, so just build a simple action set
		for(int i = 0; i < penVec.size(); i++){
			if(min == -1 && penVec[i].first >=0) {
				min = penVec[i].first;
				minInd = i;
			}else{
				if(min > penVec[i].first && penVec[i].first >= 0) {
					minInd = i;
					min =penVec[i].first;
				}
			}
		}
		//minimum is found. find if there are the same values in the array
		for(int i = 0; i < penVec.size(); i++){
			if(min == penVec[i].first) actions ->push_back(i);
		}
	}
	
}
//Tcarenko. To rollback the action (undo the action)
void NoximMapping::shrink_expand_rect_rollback(NoximRectangle *rect, direction action){
	//check the action and do the opposite one
	if(action < leftUpE) (int)action%2 == 0? shrink_expand_rect(rect, (direction)((int)action+1)) : shrink_expand_rect(rect, (direction)((int)action-1));
	else{
		bool modified = true;
		switch(action)
		{//do the opposite
			case leftUpE:		if(rect ->ldCorner.X < rect ->ruCorner.X)					rect ->ldCorner.X++; 
								else														modified = false;
								if(rect ->ruCorner.Y > rect ->ldCorner.Y && modified)		rect ->ruCorner.Y--;
								break;

			case leftDownE:		if(rect ->ldCorner.X < rect ->ruCorner.X)					rect ->ldCorner.X++; 
								else														modified = false;
								if(rect ->ldCorner.Y < rect ->ruCorner.Y && modified)		rect ->ldCorner.Y++;
								break;
		
			case rightUpE:		if(rect ->ruCorner.X >  rect ->ldCorner.X)					rect ->ruCorner.X--;
								else														modified = false;
								if(rect ->ruCorner.Y > rect ->ldCorner.Y && modified)		rect ->ruCorner.Y--;
								break;

			case rightDownE:	if(rect ->ruCorner.X >  rect ->ldCorner.X)					rect ->ruCorner.X--;
								else														modified = false;
								if(rect ->ldCorner.Y < rect ->ruCorner.Y && modified)		rect ->ldCorner.Y++;
								break;

		}
	}
}
//Tcarenko. Function for expanding/shrinking a rectangle
bool NoximMapping::shrink_expand_rect(NoximRectangle *rect, direction dir){
	bool modified = true;
	switch(dir)
	{
		case leftE:		if(rect ->ldCorner.X != 0)									rect ->ldCorner.X--;
						else														modified = false;
						break;

		case leftS:		if(rect ->ldCorner.X < rect ->ruCorner.X)					rect ->ldCorner.X++; 
						else														modified = false;
						break;

		case rightE:	if(rect ->ruCorner.X < NoximGlobalParams::mesh_dim_x)		rect ->ruCorner.X++;
						else														modified = false;
						break;

		case rightS:	if(rect ->ruCorner.X >  rect ->ldCorner.X)					rect ->ruCorner.X--;
						else														modified = false;
						break;

		case upE:		if(rect ->ruCorner.Y < NoximGlobalParams::mesh_dim_y)		rect ->ruCorner.Y++;
						else														modified = false;
						break;

		case upS:		if(rect ->ruCorner.Y > rect ->ldCorner.Y)					rect ->ruCorner.Y--;
						else														modified = false;
						break;

		case downE:		if(rect ->ldCorner.Y != 0)									rect ->ldCorner.Y--;
						else														modified = false;
						break;

		case downS:		if(rect ->ldCorner.Y < rect ->ruCorner.Y)					rect ->ldCorner.Y++;
						else														modified = false;
						break;

		case leftUpE:	if(rect ->ldCorner.X != 0)												rect ->ldCorner.X--;
						else																	modified = false;
						if(rect ->ruCorner.Y < NoximGlobalParams::mesh_dim_y && modified)		rect ->ruCorner.Y++;
						else if(modified){
								modified = false;
								//return first action back
								rect ->ldCorner.X++;
						}
						break;if(modified)

		case leftDownE:	if(rect ->ldCorner.X != 0)												rect ->ldCorner.X--;
						else																	modified = false;
						if(rect ->ldCorner.Y != 0 && modified)									rect ->ldCorner.Y--;
						else if(modified){
								modified = false;
								//return first action back
								rect ->ldCorner.X++;
						}
						break;

		case rightUpE:	if(rect ->ruCorner.X < NoximGlobalParams::mesh_dim_x)					rect ->ruCorner.X++;
						else																	modified = false;
						if(rect ->ruCorner.Y < NoximGlobalParams::mesh_dim_y && modified)		rect ->ruCorner.Y++;
						else if(modified){
								modified = false;
								//return first action back
								rect ->ruCorner.X--;
						}
						break;

		case rightDownE:	if(rect ->ruCorner.X < NoximGlobalParams::mesh_dim_x)				rect ->ruCorner.X++;
							else																modified = false;
							if(rect ->ldCorner.Y != 0 && modified)								rect ->ldCorner.Y--;
							else if(modified){
									modified = false;
									//return first action back
									rect ->ruCorner.X--;
							}
							break;
	}
	return modified;
}

//Tcarenko.moves rect
bool NoximMapping::move_rect(NoximRectangle *rect, direction dir)
{
	bool modified = true;
	switch(dir)
	{
		case leftE	:	
			modified = shrink_expand_rect(rect, leftE);
			if(modified) shrink_expand_rect(rect, rightS);
			break;
		case rightE :
			modified = shrink_expand_rect(rect, rightE);
			if(modified) shrink_expand_rect(rect, leftS);
			break;
		case upE	:
			modified = shrink_expand_rect(rect, upE);
			if(modified) shrink_expand_rect(rect, downS);
			break;
		case downE	:
			modified = shrink_expand_rect(rect, downE);
			if(modified) shrink_expand_rect(rect, upS);
			break;
	}
	return modified;
}
//Tcarenko. returns true if the area has empty nodes
bool NoximMapping::hasEmptyNodes(NoximRectangle rect, NoximInvokedApplication* app)
{
	int emptyNodes = 0;
	rectCountPenalty(rect, app, emptyNodes);
	if(emptyNodes == 0) return false;
	return true;
}