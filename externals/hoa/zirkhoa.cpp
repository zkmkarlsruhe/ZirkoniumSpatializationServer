#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "m_pd.h"

#include "Hoa.hpp"

typedef hoa::Encoder<hoa::Hoa3d, float>::Basic Encoder3D;
typedef hoa::Encoder<hoa::Hoa2d, float>::Basic Encoder2D;

typedef hoa::Decoder<hoa::Hoa2d, float>::Regular Decoder2D;
typedef hoa::Decoder<hoa::Hoa3d, float>::Regular Decoder3D;

typedef hoa::Wider<hoa::Hoa2d, float> Wider2D;
typedef hoa::Wider<hoa::Hoa3d, float> Wider3D;

typedef hoa::Optim<hoa::Hoa2d, float>::MaxRe OptimMaxRe2D;
typedef hoa::Optim<hoa::Hoa3d, float>::MaxRe OptimMaxRe3D;

typedef hoa::Optim<hoa::Hoa2d, float>::InPhase OptimInPhase2D;
typedef hoa::Optim<hoa::Hoa3d, float>::InPhase OptimInPhase3D;

typedef enum optim{
	OPTIM_BASIC,
	OPTIM_MAXRE,
	OPTIM_INPHASE
} optim;

typedef struct zirkhoa{
    t_object x_ob;
    optim x_optim;
    float x_azimuth;
	float x_elevation;
    t_inlet *x_inlet;
    t_outlet *x_outlet;
    t_outlet *x_info;
    int x_dimension;
    Encoder2D *x_encoder2D;
    Decoder2D *x_decoder2D;
    Encoder3D *x_encoder3D;
    Decoder3D *x_decoder3D;
    Wider2D *x_wider2D;
    Wider3D *x_wider3D;
    OptimMaxRe2D *x_optimMaxRe2D;
    OptimMaxRe3D *x_optimMaxRe3D;
    OptimInPhase2D *x_optimInPhase2D;
    OptimInPhase3D *x_optimInPhase3D;
} t_zirkhoa;

t_class *zirkhoa_class;

static void allocate_processors(t_zirkhoa *x, unsigned long order);
static void dealloc_decoders(t_zirkhoa *x);

static void zirkhoa_list2D(t_zirkhoa *x, t_atom *argv );
static void zirkhoa_list3D(t_zirkhoa *x, t_atom *argv );
static void zirkhoa_list(t_zirkhoa *x, t_symbol *dummy,int argc, t_atom *argv );
static void zirkhoa_dimension(t_zirkhoa *x, t_floatarg f);
static void zirkhoa_getDimension(t_zirkhoa *x);
static void zirkhoa_order(t_zirkhoa *x, t_floatarg f);
static void zirkhoa_getOrder(t_zirkhoa *x);
static void zirkhoa_getHarmonics(t_zirkhoa *x);
static void zirkhoa_getPosition(t_zirkhoa *x);
static void zirkhoa_getBFormat(t_zirkhoa *x);
static void zirkhoa_speakers(t_zirkhoa *x, t_symbol *selector, int argc, t_atom *argv );
static void zirkhoa_getSpeakers(t_zirkhoa *x);
static void *zirkhoa_new(void);
static void zirkhoa_free(t_zirkhoa *x);
static void dealloc_processors(t_zirkhoa *x);

static void zirkhoa_free(t_zirkhoa *x){

	dealloc_processors(x);

	if(x->x_decoder2D){
		delete x->x_decoder2D;
		x->x_decoder2D = NULL;
	}if(x->x_decoder3D){
    	delete x->x_decoder3D;
    	x->x_decoder2D = NULL;
    }
}

// **************************** alloc / dealloc **************************** //

static void dealloc_processors(t_zirkhoa *x){
	if(x->x_encoder2D){
	    delete x->x_encoder2D;
	    x->x_encoder2D = NULL;
	}if(x->x_encoder3D){
		delete x->x_encoder3D;
		x->x_encoder3D = NULL;
	}if(x->x_wider2D){
    	delete x->x_wider2D;
    	x->x_wider2D = NULL;
    }if(x->x_wider3D){
    	delete x->x_wider3D;
    	x->x_wider3D = NULL;
    }if(x->x_optimMaxRe2D){
	    delete x->x_optimMaxRe2D;
	    x->x_optimMaxRe2D = NULL;
	}if(x->x_optimMaxRe3D){
	    delete x->x_optimMaxRe3D;
		x->x_optimMaxRe3D = NULL;
	}if(x->x_optimInPhase2D){
	    delete x->x_optimInPhase2D;
	    x->x_optimInPhase2D = NULL;
	}if(x->x_optimInPhase3D){
	    delete x->x_optimInPhase3D;
	    x->x_optimInPhase3D = NULL;
	}
}

static void allocate_processors(t_zirkhoa *x, unsigned long order){
    	
	x->x_encoder2D = new Encoder2D(order);
	x->x_encoder3D = new Encoder3D(order);
	
	x->x_wider2D = new Wider2D(order);
	x->x_wider3D = new Wider3D(order);

	x->x_optimMaxRe2D = new OptimMaxRe2D(order);
	x->x_optimMaxRe3D = new OptimMaxRe3D(order);
	
	x->x_optimInPhase2D = new OptimInPhase2D(order);
	x->x_optimInPhase3D = new OptimInPhase3D(order);
}

// every time dimension or order is changed invoke this and 
// force users to redefine the speakers

static void dealloc_decoders(t_zirkhoa *x ){
	if(x->x_decoder2D){
		delete x->x_decoder2D;
		x->x_decoder2D = NULL;
	}
	if(x->x_decoder3D){
		delete x->x_decoder3D;
		x->x_decoder3D = NULL;
	}
}

// **************************** list **************************** //

static void zirkhoa_list2D(t_zirkhoa *x, t_atom *argv ){

    if(argv[0].a_type != A_FLOAT){
        post("wrong type of arguments");
    	return;
    }

    x->x_azimuth = argv[0].a_w.w_float;
    x->x_encoder2D->setAzimuth(x->x_azimuth);

    unsigned long numHarmonics = x->x_encoder2D->getNumberOfHarmonics();
    float * bFormat = (float*)malloc(sizeof(float) * numHarmonics);
    float * wFormat = (float*)malloc(sizeof(float) * numHarmonics);

    float reference = 1.0;
    x->x_encoder2D->process(&reference, bFormat);
    x->x_wider2D->process(bFormat, wFormat);

    unsigned long numSpeakers  = x->x_decoder2D->getNumberOfPlanewaves();

    float * samples = (float*)malloc(sizeof(float) * numSpeakers);
    t_atom args[2];

    if(x->x_optim == OPTIM_BASIC){
    	x->x_decoder2D->process(wFormat, samples);
    }else{
    	float * oFormat = (float*)malloc(sizeof(float) * numHarmonics);
    	if(x->x_optim == OPTIM_MAXRE){
    		x->x_optimMaxRe2D->process(wFormat, oFormat);
    	}else{
    		x->x_optimInPhase2D->process(wFormat, oFormat);
    	}
    	x->x_decoder2D->process(oFormat, samples);
    	if(oFormat) free(oFormat);
    }

    for(int i = 0; i < numSpeakers; i++){
    	SETFLOAT(&args[0], i);
    	SETFLOAT(&args[1], samples[i]);
        outlet_list(x->x_outlet, gensym("dummy"), 2, &args[0]);
    }

    if(bFormat) free(bFormat);
    if(wFormat) free(wFormat);
    if(samples) free(samples);
}

static void zirkhoa_list3D(t_zirkhoa *x, t_atom *argv ){

    if((argv[0].a_type != A_FLOAT) || (argv[1].a_type != A_FLOAT)){
        post("wrong type of arguments");
    	return;
    }

    x->x_azimuth =  argv[0].a_w.w_float;
    x->x_elevation =  argv[1].a_w.w_float;

    x->x_encoder3D->setAzimuth(x->x_azimuth);
    x->x_encoder3D->setElevation(x->x_elevation);

    unsigned long numHarmonics = x->x_encoder3D->getNumberOfHarmonics();
    float * bFormat = (float*)malloc(sizeof(float) * numHarmonics);
    float * wFormat = (float*)malloc(sizeof(float) * numHarmonics);

    float reference = 1.0;
    x->x_encoder3D->process(&reference, bFormat);
    x->x_wider3D->process(bFormat, wFormat);

    unsigned long numSpeakers  = x->x_decoder3D->getNumberOfPlanewaves();
    float *samples = (float*)malloc(sizeof(float) * numSpeakers);
    t_atom *args = (t_atom *)malloc(sizeof(t_atom) * numSpeakers);

    if(x->x_optim == OPTIM_BASIC){
    	x->x_decoder3D->process(wFormat, samples);
    }else{
    	float * oFormat = (float*)malloc(sizeof(float) * numHarmonics);
    	if(x->x_optim == OPTIM_MAXRE){
    		x->x_optimMaxRe3D->process(wFormat, oFormat);
    	}else{
    		x->x_optimInPhase3D->process(wFormat, oFormat);
    	}
    	x->x_decoder3D->process(oFormat, samples);
    	if(oFormat) free(oFormat);
    }
    for(int i = 0; i < numSpeakers; i++){
    	SETFLOAT(&args[0], i);
    	SETFLOAT(&args[1], samples[i]);
        outlet_list(x->x_outlet, gensym("dummy"), 2, &args[0]);
    }

    if(samples) free(samples);
    if(bFormat) free(bFormat);
    if(wFormat) free(wFormat);
}

static void debugAddress(t_zirkhoa *x){
	post("address: enc2D:%d enc3D:%d \ndec2D:%d dec3D:%d \nwid2D:%d wid3D:%d \noptMaxRe2D:%d optMaxRe3D:%d\n optInP2D:%d optInP3D:%d", 
		x->x_encoder2D,x->x_encoder3D,x->x_decoder2D,x->x_decoder3D,x->x_wider2D,x->x_wider3D,x->x_optimMaxRe2D,x->x_optimMaxRe3D,x->x_optimInPhase2D,x->x_optimInPhase3D);
}

static void zirkhoa_list(t_zirkhoa *x, t_symbol *dummy, int argc, t_atom *argv ){
	//debugAddress(x);

    if(x->x_dimension == 0){
		post("setup dimension first");
		return;
	}
	if(x->x_dimension == 2){
	    if(!x->x_encoder2D){
	    	post("set up order");
	    	return;
	    }
	    if(!x->x_decoder2D){
	        post("set up speakers");
	        return;
	    }
	}else{
	    if(!x->x_encoder3D){
	    	post("set up order");
	    	return;
	    }
	    if(!x->x_decoder3D){
	        post("set up speakers");
	        return;
	    }
	}

	if( argc == 0 ){
    	post("insufficient number of argument");
    	return;
    }

    if( x->x_dimension == 2 ){
    	if(argc > 1 ){
    		argc = 1; // the rest will be ignored
    	}
    	zirkhoa_list2D(x, argv);
    }else {
    	if(argc != 2){
			post("wrong number of argument. must be 2.");
    		return;
    	}
    	zirkhoa_list3D(x, argv);
    }
}

// **************************** dimension **************************** //

static void zirkhoa_dimension(t_zirkhoa *x, t_floatarg f){
	int dim = (int)f;
	int previousDimension = x->x_dimension;

	if(!(dim == 2 || dim == 3)){
		post("dimension must be 2 or 3");
		return;
	}
	x->x_dimension = dim;

	dealloc_decoders(x);
}

static void zirkhoa_getDimension(t_zirkhoa *x){
	t_atom dimAtom;
    SETFLOAT(&dimAtom, x->x_dimension);
    outlet_anything(x->x_info, gensym("dimension"), 1, &dimAtom);
}

// **************************** order **************************** //

static void zirkhoa_order(t_zirkhoa *x, t_floatarg f){
	if(x->x_dimension == 0){
		post("setup dimension first!");
		return;
	}

    unsigned long order = (unsigned long)f;
    if(order < 1 || order > 7){
        post("The order must be between 1 and 7.");
        return;
    }
	dealloc_processors(x);
    allocate_processors(x, order);
    dealloc_decoders(x);
}

static void zirkhoa_getOrder(t_zirkhoa *x){
    t_atom orderAtom;

    if(x->x_dimension == 2){
	    SETFLOAT(&orderAtom, (float)x->x_encoder2D->getDecompositionOrder());
    }else{
	    SETFLOAT(&orderAtom, (float)x->x_encoder3D->getDecompositionOrder());
	}
    outlet_anything(x->x_info, gensym("order"), 1, &orderAtom);
}

// **************************** harmonics **************************** //

static void zirkhoa_getHarmonics(t_zirkhoa *x){
    t_atom harmonicAtom;
    if(x->x_dimension == 2){
	   	 SETFLOAT(&harmonicAtom, (float)x->x_encoder2D->getNumberOfHarmonics());
	}else{
		 SETFLOAT(&harmonicAtom, (float)x->x_encoder3D->getNumberOfHarmonics());
	}
    outlet_anything(x->x_info, gensym("harmonics"), 1, &harmonicAtom);
}

// **************************** position **************************** //

static void zirkhoa_getPosition(t_zirkhoa *x){

	if(x->x_dimension == 2){
		if(!x->x_encoder2D){
			post("setup order and speaker first");
			return;
		}
		t_atom positionAtom;
	    SETFLOAT(&positionAtom, (float)x->x_encoder2D->getAzimuth());
	    outlet_anything(x->x_info, gensym("position"), 1, &positionAtom);

	}else{
		if(!x->x_encoder3D){
			post("setup order and speaker first");
			return;
		}
	    t_atom positionAtom[2];
	    SETFLOAT(&positionAtom[0], (float)x->x_encoder3D->getAzimuth());
	    SETFLOAT(&positionAtom[1], (float)x->x_encoder3D->getElevation());
	    outlet_anything(x->x_info, gensym("position"), 2, &positionAtom[0]);
	}	
}

// **************************** b-format **************************** //

static void zirkhoa_getBFormat(t_zirkhoa *x){
	unsigned long numHarmonics;
	float * bFormat, * wFormat, *oFormat;
	if(x->x_dimension == 2){
		if(!x->x_encoder2D){
			post("setup order and speaker first");
			return;
		}
	    numHarmonics = x->x_encoder2D->getNumberOfHarmonics();
   		bFormat = (float*)malloc(sizeof(float) * numHarmonics);
   		wFormat = (float*)malloc(sizeof(float) * numHarmonics);
   		oFormat = (float*)malloc(sizeof(float) * numHarmonics);

   		float reference = 1.0;
  	  	x->x_encoder2D->process(&reference, bFormat);
  	  	x->x_wider2D->process(bFormat, wFormat);
  	  	if(x->x_optim == OPTIM_BASIC){
  	  		for(int i = 0; i < numHarmonics; i++){
  	  			oFormat[i] = wFormat[i];
  	  		}
  	  	}else{
  	  		if(x->x_optim == OPTIM_MAXRE){
  	  			x->x_optimMaxRe2D->process(wFormat, oFormat);
  	  		}else{
  	  			x->x_optimInPhase2D->process(wFormat, oFormat);
  	  		}
  	  	}
	}else{

		if(!x->x_encoder3D){
			post("setup order and speaker first");
			return;
		}

	    numHarmonics = x->x_encoder3D->getNumberOfHarmonics();
   		bFormat = (float*)malloc(sizeof(float) * numHarmonics);
   		wFormat = (float*)malloc(sizeof(float) * numHarmonics);
   		oFormat = (float*)malloc(sizeof(float) * numHarmonics);

   		float reference = 1.0;
  	  	x->x_encoder3D->process(&reference, bFormat);
  	  	x->x_wider3D->process(bFormat, wFormat);
  	  	if(x->x_optim == OPTIM_BASIC){
  	  		for(int i = 0; i < numHarmonics; i++){
  	  			oFormat[i] = wFormat[i];
  	  		}
  	  	}else{
  	  		if(x->x_optim == OPTIM_MAXRE){
  	  			x->x_optimMaxRe3D->process(wFormat, oFormat);
  	  		}else{
  	  			x->x_optimInPhase3D->process(wFormat, oFormat);
  	  		}
  	  	}
	}

	t_atom * args = (t_atom*)malloc(sizeof(t_atom) * numHarmonics);
 	for(int i = 0; i < numHarmonics; i++){
    	args[i].a_type = A_FLOAT;
		args[i].a_w.w_float =oFormat[i];
	}
	outlet_anything(x->x_info, gensym("b-format"), numHarmonics, &args[0]);
	if(bFormat) free(bFormat);
	if(wFormat) free(wFormat);
	if(oFormat) free(oFormat);
	if(args) free(args);
}

// **************************** speakers **************************** //

static void zirkhoa_speakers(t_zirkhoa *x, t_symbol *selector, int argc, t_atom *argv ){

	if(x->x_dimension == 0){
		post("setup dimension first");
		return;
	}


	if(x->x_dimension == 2){
		if(!x->x_encoder2D){
			post("set up order first");
	        return;
		}
	    unsigned long minimumNumberOfSpeakers = x->x_encoder2D->getNumberOfHarmonics();
	   	int numSpeakers = argc;
	    if(numSpeakers  < minimumNumberOfSpeakers){
	        post("too few speakers. the minimum number of speakers is %lu.", minimumNumberOfSpeakers);
	        return;
	    }

	    unsigned long decompostionOrder = x->x_encoder2D->getDecompositionOrder();
	    if(x->x_decoder2D){
		    delete x->x_decoder2D;
		    x->x_decoder2D = NULL;
		}
	    x->x_decoder2D = new Decoder2D(decompostionOrder, numSpeakers);
	    for(int i = 0; i < numSpeakers; i++){
	        if(argv[i].a_type == A_FLOAT){
				x->x_decoder2D->setPlanewaveAzimuth(i, argv[i].a_w.w_float);
	        }else{
	            post ("argment:%d wrong type. azimuth set to 0.", index);
	            x->x_decoder2D->setPlanewaveAzimuth(i, 0);
	        }
	        x->x_decoder2D->computeRendering();
	    }
	}else{
		if(!x->x_encoder3D){
			post("set up order first");
	        return;
		}

	    if(argc % 2){
	        post("wrong number of argument, %d. should be even number.", argc);
	        return;
	    }

	    int numSpeakers = argc / 2;
	    unsigned long minimumNumberOfSpeakers = x->x_encoder3D->getNumberOfHarmonics();
	    if(numSpeakers  < minimumNumberOfSpeakers){
	        post("too few speakers. the minimum number of speakers is %lu.", minimumNumberOfSpeakers);
	        return;
	    }

	    unsigned long decompoistionOrder = x->x_encoder3D->getDecompositionOrder();
	    if(x->x_decoder3D){
		    delete x->x_decoder3D;
		    x->x_decoder3D = NULL;
		}
	    x->x_decoder3D = new Decoder3D(decompoistionOrder, numSpeakers);
	    int index = 0;
	    for(int i = 0; i < numSpeakers; i++){

	        if(argv[index].a_type == A_FLOAT){
				x->x_decoder3D->setPlanewaveAzimuth(i, argv[index].a_w.w_float);
	        }else{
	            post("argment:%d wrong type. azimuth set to 0.", index);
	            x->x_decoder3D->setPlanewaveAzimuth(i, 0);
	        }
	        
	        index++;
	        if(argv[index].a_type == A_FLOAT){
	            x->x_decoder3D->setPlanewaveElevation(i, argv[index].a_w.w_float);
	        }else{
	            post("argment:%d wrong type. elevation set to 0.", index);
	            x->x_decoder3D->setPlanewaveElevation(i, argv[index].a_w.w_float);
	        }
	        index++;

	        x->x_decoder3D->computeRendering();
	    }
	}
}

static void zirkhoa_getSpeakers(t_zirkhoa *x){

	if(x->x_dimension == 0){
		post("setup dimension first");
		return;
	}
	t_atom * args;
	unsigned long numberOfSpeakers;
	unsigned long numArgs;
	if(x->x_dimension == 2){
		numberOfSpeakers = x->x_decoder2D->getNumberOfPlanewaves();
	    numArgs= numberOfSpeakers;
	    args = (t_atom*)malloc(sizeof(t_atom) * numArgs );
	    for(int i = 0; i < numberOfSpeakers; i++){
    	    args[i].a_type = A_FLOAT;
        	args[i].a_w.w_float = x->x_decoder2D->getPlanewaveAzimuth(i);
    	}
	}else{
		numberOfSpeakers = x->x_decoder3D->getNumberOfPlanewaves();
	    numArgs= 2 * numberOfSpeakers;
	    args = (t_atom*)malloc(sizeof(t_atom) * numArgs );
	    int argIndex = 0;
	    for(int i = 0; i < numberOfSpeakers; i++){
    	    args[argIndex].a_type = A_FLOAT;
        	args[argIndex++].a_w.w_float = x->x_decoder3D->getPlanewaveAzimuth(i);

	        args[argIndex].a_type = A_FLOAT;
	        args[argIndex++].a_w.w_float = x->x_decoder3D->getPlanewaveElevation(i);
    	}
    }
    outlet_anything(x->x_info, gensym("speakers"), numArgs, args);

    if(args) free(args);
    
}

// **************************** widening **************************** //

static void zirkhoa_widening(t_zirkhoa *x, float widening){

	if(x->x_dimension == 0){
		post("setup dimension first");
		return;
	}

	if(x->x_dimension == 2){
		if(!x->x_encoder2D){
			post("setup order first");
	        return;
		}
		if(!x->x_decoder2D){
			post("setup speakers first");
			return;
		}
		x->x_wider2D->setWidening(widening);
		t_atom azimuthAtom;
		SETFLOAT(&azimuthAtom, (float)x->x_azimuth);
		zirkhoa_list2D(x, &azimuthAtom);

	}else{
		if(!x->x_encoder3D){
			post("setup order first");
			return;
		}
		if(!x->x_decoder3D){
			post("setup speakers first");
			return;
		}
		x->x_wider3D->setWidening(widening);
		t_atom positionAtom[2];
		SETFLOAT(&positionAtom[0], (float)x->x_azimuth);
		SETFLOAT(&positionAtom[1], (float)x->x_elevation);
		zirkhoa_list3D(x, &positionAtom[0]);
	}

}

static void zirkhoa_getWidening(t_zirkhoa *x){

	t_atom wideningAtom;
	if(x->x_dimension == 2){
		if(!x->x_encoder2D){
			post("setup order first");
			return;
		}
	    SETFLOAT(&wideningAtom, (float)x->x_wider2D->getWidening());
	}else{
		if(!x->x_encoder3D){
			post("setup order first");
			return;
		}
	    SETFLOAT(&wideningAtom, (float)x->x_wider3D->getWidening());
	}	
	outlet_anything(x->x_info, gensym("widening"), 1, &wideningAtom);
}

// **************************** optim **************************** //

static void zirkhoa_optim(t_zirkhoa *x, t_symbol* optim){

	if(x->x_dimension == 0){
		post("setup dimension first");
		return;
	}

	if(x->x_dimension == 2){
		if(!x->x_encoder2D){
			post("setup order first");
	        return;
		}
	}else{
		if(!x->x_encoder3D){
			post("setup order first");
			return;
		}
	}

	const char* modeName = optim->s_name;
	if(strcmp(modeName, "basic") == 0){
		x->x_optim = OPTIM_BASIC;
	}else if(strcmp(modeName, "maxre") == 0){
		x->x_optim = OPTIM_MAXRE;
	}else if(strcmp(modeName, "inphase") == 0){
		x->x_optim = OPTIM_INPHASE;
	}else{
		post("no such optim mode %s. use basic, maxre or inphase.", modeName);
	}
}

static void zirkhoa_getOptim(t_zirkhoa *x){

	optim mode = x->x_optim;
	t_atom optimAtom;
	switch(mode){
		case OPTIM_BASIC:{
			SETSYMBOL(&optimAtom, gensym("basic"));
			break;
		}
		case OPTIM_MAXRE:{
			SETSYMBOL(&optimAtom, gensym("maxre"));
			break;
		}
		case OPTIM_INPHASE:{
			SETSYMBOL(&optimAtom, gensym("inphase"));
			break;
		}
	}
	outlet_anything(x->x_info, gensym("optim"), 1, &optimAtom);
		
}

// **************************** new **************************** //

static void *zirkhoa_new(void){
    t_zirkhoa *x = (t_zirkhoa *)pd_new(zirkhoa_class);
    x->x_inlet = inlet_new(&x->x_ob, &x->x_ob.ob_pd ,  gensym("float"), gensym("widening"));
    x->x_outlet = outlet_new(&x->x_ob, gensym("list"));
    x->x_info = outlet_new(&x->x_ob, 0); // outlet for any message
	x->x_dimension = 0;
	x->x_optim = OPTIM_BASIC;
	
	x->x_encoder2D = NULL;
	x->x_encoder3D = NULL;
	x->x_decoder2D = NULL;

	x->x_decoder3D = NULL;
	x->x_optimMaxRe2D = NULL;
	x->x_optimMaxRe3D = NULL;
	x->x_optimInPhase2D = NULL;
	x->x_optimInPhase3D = NULL;
	x->x_wider2D = NULL;
	x->x_wider3D = NULL;

	//debugAddress(x);

    return (void *)x;
}

extern "C"{

void zirkhoa_setup(void){
    zirkhoa_class = (t_class*)class_new(gensym("zirkhoa"),
                              (t_newmethod)zirkhoa_new,
                              (t_method)zirkhoa_free, sizeof(t_zirkhoa), CLASS_DEFAULT, (t_atomtype)0);
    class_addmethod(zirkhoa_class, (t_method)zirkhoa_dimension, gensym("dimension"), A_FLOAT, (t_atomtype)0);
    class_addmethod(zirkhoa_class, (t_method)zirkhoa_getDimension, gensym("getDimension"), (t_atomtype)0);
    class_addmethod(zirkhoa_class, (t_method)zirkhoa_order, gensym("order"), A_FLOAT, (t_atomtype)0);
    class_addmethod(zirkhoa_class, (t_method)zirkhoa_getOrder, gensym("getOrder"), (t_atomtype)0);
    class_addmethod(zirkhoa_class, (t_method)zirkhoa_getHarmonics, gensym("getHarmonics"), (t_atomtype)0);
    class_addmethod(zirkhoa_class, (t_method)zirkhoa_getBFormat, gensym("getBFormat"), (t_atomtype)0);
    class_addmethod(zirkhoa_class, (t_method)zirkhoa_speakers, gensym("speakers"), A_GIMME, (t_atomtype)0);
    class_addmethod(zirkhoa_class, (t_method)zirkhoa_getSpeakers, gensym("getSpeakers"), (t_atomtype)0);
    class_addmethod(zirkhoa_class, (t_method)zirkhoa_getPosition, gensym("getPosition"), (t_atomtype)0);
    class_addmethod(zirkhoa_class, (t_method)zirkhoa_widening, gensym("widening"), A_FLOAT, (t_atomtype)0);
    class_addmethod(zirkhoa_class, (t_method)zirkhoa_getWidening, gensym("getWidening"), (t_atomtype)0);
    class_addmethod(zirkhoa_class, (t_method)zirkhoa_optim, gensym("optim"), A_SYMBOL, (t_atomtype)0);
    class_addmethod(zirkhoa_class, (t_method)zirkhoa_getOptim, gensym("getOptim"), (t_atomtype)0);

    class_addlist(zirkhoa_class, (t_method)zirkhoa_list);
}

} // extern
