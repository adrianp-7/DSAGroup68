#ifndef DATA_SOURCE_H
#define DATA_SOURCE_H

#include <vector>
#include <string>
#include <unordered_map>
#include <set>

using namespace std;

#include "JSONutil.h"
#include "data_src/MovieActorWikidata.h"
#include "ColorGrid.h"
#include "base64.h"
#include "GraphAdjList.h"
#include "ServerComm.h"
#include "Bridges.h"
#include "rapidjson/document.h"
#include "assert.h"
#include "rapidjson/error/en.h"
#include "Cache.h"
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>

#include <cache.h>

namespace bridges {
	using namespace bridges::dataset;
	using namespace bridges::datastructure;

	/**
	 * @brief This class provides an API to various data sources used in BRIDGES.
	 *
	 * BRIDGES currently supports a few external datasets for use with BRIDGES
	 * assignments: USGIS Earthquake Tweeet streaming data feed, IMDB (file),
	 * Book metadata collection, IGN Game Data, Shakespear book/poem meta data, etc.

	 * Functions are provided that access a user specified number of data
	 * records; objects of the appropriate type are returned as a list.
	 *
	 * Outputs more information if the environment variable
	 * FORCE_BRIDGES_DATADEBUG exists, regardless of what it is
	 * set too.
	 *
	 */

	class DataSource {

		private:

			bool debug_flag = false;

			int debug() const {
				return debug_flag;
			}

			void set_debug_flag() {
				debug_flag = true;
			}

			bridges::Bridges* bridges_inst;
			bridges::lruCache my_cache;

			string getOSMBaseURL() const {
				if (sourceType == "local")
					return "http://localhost:3000/";

				return "http://bridges-data-server-osm.bridgesuncc.org/";
			}
			string getElevationBaseURL() const {
				if (sourceType == "local")
					return "http://localhost:3000/";

				return "http://bridges-data-server-elevation.bridgesuncc.org/";
			}
			string getGutenbergBaseURL() const {
				if (sourceType == "local")
					return "http://localhost:3000/";

				if (sourceType == "testing")
					return "http://bridges-data-server-gutenberg-t.bridgesuncc.org/";

				return "http://bridges-data-server-gutenberg.bridgesuncc.org/";
			}
			string  getRedditURL() {
				if (sourceType == "testing")
					return "http://bridges-data-server-reddit-t.bridgesuncc.org";
				else if (sourceType == "local")
					return "http://localhost:9999";
				else
					return "http://bridges-data-server-reddit.bridgesuncc.org";
			}

			string sourceType = "live";

			string getUSCitiesURL() {
				return "http://bridgesdata.herokuapp.com/api/us_cities";
			}

	  void defaultDebug() {
	    char* force = getenv("FORCE_BRIDGES_DATADEBUG");
	    if (force != nullptr)
	      set_debug_flag();

	  }
	  
		public:
			DataSource(bridges::Bridges* br = nullptr)
			  : bridges_inst(br), my_cache(120) {defaultDebug();}

			DataSource(bridges::Bridges& br )
				: DataSource(&br) {defaultDebug();}

			/**
			 *  @brief set data server type
			 *
			 *	@param string  data server type, can be 'live', 'testing', or 'local')
			 *  @throws exception if incorrect type is passed
			 *
			 */
			void setSourceType(string type) {
				if ( !((type == "live") || (type == "testing") || (type == "local")))
					throw "Incorrect data server type. Must be live, testing or local";
				if ((type == "testing") || (type == "local"))
					debug_flag = true;

				sourceType = type;
			}

			/**
			 * @brief  Retrieves US city data based on a set of filtering parameters
			 *
			 * @param  params  this represents a specification of the filtering
			 *			parameters provided as a map. Multiple parameters will result
			 * 			in filtering as a combination (intersection)
			 *			Available parameters and their  types are as follows:
			 *         'city' : string
			 *         'state' : string
			 *         'country' : string
			 *         'time_zone' : string
			 *         'min_elev' : integer
			 *         'max_elev' : integer
			 *         'min_pop' : integer
			 *         'max_pop' : integer
			 *         'min_lat' : float -- Lat minimum
			 *         'max_lat' : float -- Lat maximum
			 *         'min_long' : float -- Lat minimum
			 *         'max_long' : float -- Lat maximum
			 *		   'limit' : integer -- max number of cities to return
			 *
			 */

			/**
			 *
			 *  @brief Get meta data of the IGN games collection.
			 *
			 *  This function retrieves  and formats the data into a list of
			 *  Game objects
			 *
			 *  @throws Exception if the request fails
			 *
			 *  @return a list of Game objects,
			 *
			 */

			/**
			 *  @brief Get ActorMovie IMDB Data
			 *  Data is retrieved, formatted into a list of ActorMovieIMDB objects
			 *
			 *  @param number the number of actor/movie pairs, but currently unused,
			 *      returns all records.
			 *  @throws Exception if the request fails
			 *
			 *  @return a list of ActorMovieIMDB objects, but only actor and
			 *			movie fields in this version
			 */


			/**
			 *  @brief Get ActorMovie IMDB Data
			 *  Data is retrieved, formatted into a list of ActorMovieIMDB objects
			 *
			 *  @throws Exception if the request fails
			 *
			 *  @return a list of ActorMovieIMDB objects, consisting of
			 *	actor name, movie name, movie genre and movie rating is returned.
			 *
			 */


			/**
			 *  @brief Get USGS earthquake data
			 *  USGS Tweet data (https://earthquake.usgs.gov/earthquakes/map/)
			 *  retrieved, formatted into a list of EarthquakeUSGS objects
			 *
			 *  @param number the number of earthquake records retrieved,
			 *		limited to 5000
			 *  @throws Exception if the request fails
			 *
			 *  @return a list of earthquake records
			 */

			/**
			 *  @brief Get data of Shakespeare works (plays, poems)
			 *
			 *  This function retrieves  and formats the data into a
			 *  a list of Shakespeare objects.
			 *
			 *  @throws Exception if the request fails
			 *
			 *  @param type can be "plays", "poems", or "". If this is
			 *		specified, then only these types of works are retrieved.
			 *  @param textonly  if this is set, then only the text is retrieved. (that is to say punctuation is stripped out)
			 *
			 *  @return an array of Shakespeare objects
			 *
			 */

			/**
			 *
			 *  @brief Get data of a particular songs (including lyrics) using
			 *	 the Genius API
			 *  (https://docs.genius.com/), given the song title and artist name.
			 *	Valid endpoints:  http://bridgesdata.herokuapp.com/api/songs/find/
			 *	Valid queryParams: song title, artist name
			 *
			 *  This function retrieves  and formats the data into a
			 *  Song object. The song if not cached in the local DB is queried
			 *  and added to the DB
			 *
			 * @param songTitle title of the song. inexact is ok, will be matched by genisu
			 * @param artistName name of artist. empty string if unspecified. If specified, must be exact
			 *  @throws Exception if the request fails
			 *
			 *  @return a Song object,
			 *
			 */

			/**
			 *
			 *  @brief Get data of the songs (including lyrics) using the Genius API
			 *  https://docs.genius.com/
			 *	Valid endpoints:  https://bridgesdata.herokuapp.com/api/songs/
			 *
			 *  This function retrieves  and formats the data into a list of
			 *  Song objects. This version of the API retrieves all the cached
			 *  songs in the local DB.
			 *
			 *  @throws Exception if the request fails
			 *
			 *  @return a list of Song objects,
			 *
			 */

			/**
			 *
			 *  @brief Get meta data of the Gutenberg book collection.
			 *  This function retrieves,  and formats the data into a list of
			 *  GutenbergBook objects
			 *
			 *  @throws Exception if the request fails
			 *
			 *  @return a list of GutenbergBook objects,
			 *
			 */

		public:
			/**
			 *
			 *  @brief Get meta data of a single Gutenberg book
			 *  This function retrieves,  and formats the data into a list of
			 *  GutenbergBook objects
			 *
			 *
			 *  @param id  Gutenberg book id
			 *
			 *  @return metadata of the book
			 *
			 */

			/**
			 * @brief Search the gutenberg data for retrieving meta
			 *   data of books matching a string and a category
			 *
			 *  Data is retrieved  into a vector of book records
			 *
			 *  @param term  a string that matches the category
			 *  @param category  category can be any book attribute (title, genre,
			 *					date, Library of Congress class, language)
			 */


			/**
			 * @brief Get the full text of the book with the provided id
			 *
			 *  Data is retrieved  into a text string
			 *
			 *  @param id  id of Gutenberg book
			 */


			/**
			 * @brief Retrieves the CDC dataset of Cancer Incidence.
			 *  Data is retrieved  into a vector of records
			 * See CancerIncidence class for more information
			 *
			 */

			/**
			 *
			 * @brief Retrieves the Open Street Map data from a prebuilt JSON of OSM
			 * dataset.
			 *
			 * @param osm_json JSON string
			 */

		public:

			/**
			 *
			 *  @brief Get OpenStreetMap data given a bounding rectangle of
			 *	lat/long values.
			 *
			 *   The function also take a level of detail
			 *   which can be anything in ["motorway",
			 *   "trunk", "primary", "secondary",
			 *   "tertiary, "unclassified", "residential",
			 *   "living_street", "service", "trails",
			 *   "walking", "bicycle" ]
			 *
			 *  @param lat_min  latitude minimum
			 *  @param long_min  longitude minimum
			 *  @param lat_max   latitude maximum
			 *  @param long_max   longitude maximum
			 *  @param level      data resolution
			 *  @throws Exception if the request fails
			 *
			 *  @return an OSMData object
			 *
			 */

			/**
			 * This method retrieves the specified amenity related data given a
			 * bounding box of a region, from a Open Street map
			 *
			 *  @param minLat  minimum latitude
			 *  @param minLon  minimumm longitude
			 *  @param maxLat  maximum latitude
			 *  @param maxLon  maximum longitude
			 *  @param amenity  amenity type
			 *
			 * 	@return vector<Amenity> containing list of amenities
			 *
			 * 	@throws exception
			 */


			/**
			 * This method retrieves the specified amenity related data given a location
			 * from a specified openstreet mmap location
			 *
			 *  @param location city/town from where amenity data is sought
			 *  @param amenity  amenity type
			 *
			 * @return vector<Amenity> containing list of amenities
			 *
			 *  @throws exception
			 */


			/**
			 * @brief Parses  the amenity string and returns an AmenityData
			 *			object
			 *
			 * @param amenity_json  string of the url that will be used
			 *	when requesting amenity data from server
			 *
			 * @return vector<Amenity> containing list of amenities
			 *
			 * @throws If there is an error parsing response from
			 *      server or is an invalid location name
			 */


			/**
			 *
			 *  Get OpenStreetMap data given a city name and resolution level
			 *
			 *  The function also take a level of detail
			 *  which can be anything in ["motorway",
			 *  "trunk", "primary", "secondary",
			 *  "tertiary, "unclassified", "residential",
			 *  "living_street", "service", "trails",
			 *  "walking", "bicycle" ]
			 *
			 *  @param location   location name (string)
			 *  @param level      data resolution
			 *
			 *  @throws Exception if the request fails
			 *
			 *  @return an OSMData object
			 *
			 */

			/**
			 * Reconstruct a GraphAdjList from an existing GraphAdjList on the Bridges server
			 *
			 * The reconstructed assignment sees vertices identified as integers in the order they are stored in the server.
			 * The data associated with a vertex is a string that comes from the label of that vertices.
			 * The data associated with an edge is the string that comes from the label of that edge.
			 * The edge weights are also reobtained from the bridges server.
			 *
			 * @param user the name of the user who uploaded the assignment
			 * @param assignment the ID of the assignment to get
			 * @param subassignment the ID of the subassignment to get
			 *
			 * @return the ColorGrid stored in the bridges server
			 *
			 */
			bridges::GraphAdjList<int, std::string> getGraphFromAssignment (
				const std::string& user,
				int assignment,
				int subassignment = 0) {

				bridges::GraphAdjList<int, std::string> gr;

				std::string s = this->getAssignment(user, assignment, subassignment);

				rapidjson::Document doc;
				doc.Parse(s.c_str());
				if (doc.HasParseError())
					throw "Malformed JSON";

				//Access doc["assignmentJSON"]
				const auto& assjson = doc.FindMember("assignmentJSON");

				if (assjson == doc.MemberEnd())
					throw "Malformed GraphAdjacencyList JSON: no assignmentJSON";

				//Access doc["assignmentJSON"]["data"]
				const auto& dataArray = assjson->value.FindMember("data");

				if (dataArray == assjson->value.MemberEnd()
					|| dataArray->value.IsArray() == false)
					throw "Malformed GraphAdjacencyList JSON: No data";

				const auto& data = dataArray->value.GetArray()[0];

				//Access doc["assignmentJSON"]["data"][0]["visual"]
				const auto& dataVisual = data.FindMember("visual");

				if (dataVisual == data.MemberEnd() ||
					dataVisual->value.IsString() == false)
					throw "Malformed GraphAdjacencyList JSON";

				std::string assignment_type = dataVisual->value.GetString();

				if (assignment_type != "GraphAdjacencyList")
					throw "Malformed GraphAdjacencyList JSON: Not a GraphAdjacencyList";

				//reconstructing vertices out of nodes, and using the optional "name" as the data associated
				{
					const auto& nodes = data.FindMember("nodes");
					if (nodes == data.MemberEnd() ||
						nodes->value.IsArray() == false)
						throw "Malformed GraphAdjacencyList JSON: malformed nodes";

					const auto& nodeArray = nodes->value.GetArray();
					int nbVertex = nodeArray.Size();
					for (int i = 0; i < nbVertex; ++i) {
						std::string name;

						const auto& vertexJSONstr = nodeArray[i];

						const auto& nameJSON = vertexJSONstr.FindMember("name");
						if (nameJSON != vertexJSONstr.MemberEnd()
							&& nameJSON->value.IsString()) {
							name = nameJSON->value.GetString();
						}
						gr.addVertex(i, name);
					}
				}

				//reconstructing links, and using "label" as data associated with the link
				{
					const auto& links = data.FindMember("links");
					if (links == data.MemberEnd() ||
						links->value.IsArray() == false)
						throw "Malformed GraphAdjacencyList JSON: malformed links";

					const auto& linkArray = links->value.GetArray();
					int nbLink = linkArray.Size();
					for (int i = 0; i < nbLink; ++i) {
						std::string name;
						int src;
						int dest;
						int wgt;

						const auto& linkJSONstr = linkArray[i];

						//checking label. Maybe does not exist? (is that field optional?)
						const auto& nameJSON = linkJSONstr.FindMember("label");
						if (nameJSON != linkJSONstr.MemberEnd()
							&& nameJSON->value.IsString()) {
							name = nameJSON->value.GetString();
						}

						//checking source
						const auto& srcJSON = linkJSONstr.FindMember("source");
						if (srcJSON == linkJSONstr.MemberEnd()
							|| srcJSON->value.IsInt() == false) {
							throw "Malformed GraphAdjacencyList JSON: malformed link";
						}
						src = srcJSON->value.GetInt();

						//checking destination
						const auto& dstJSON = linkJSONstr.FindMember("target");
						if (dstJSON == linkJSONstr.MemberEnd()
							|| dstJSON->value.IsInt() == false) {
							throw "Malformed GraphAdjacencyList JSON: malformed link";
						}
						dest = dstJSON->value.GetInt();

						//checking weight. //why is weight a mandatory parameter?
						const auto& wgtJSON = linkJSONstr.FindMember("weight");
						if (wgtJSON == linkJSONstr.MemberEnd()
							|| wgtJSON->value.IsInt() == false) {
							throw "Malformed GraphAdjacencyList JSON: malformed link";
						}
						wgt = wgtJSON->value.GetInt();

						//adding edge.
						gr.addEdge(src, dest, name);
					}
				}

				return gr;
			}

			/**Reconstruct a ColorGrid from an existing ColorGrid on the Bridges server
			 *
			 * @return the ColorGrid stored in the bridges server
			 * @param user the name of the user who uploaded the assignment
			 * @param assignment the ID of the assignment to get
			 * @param subassignment the ID of the subassignment to get
			 **/
			bridges::ColorGrid getColorGridFromAssignment(const std::string& user,
				int assignment,
				int subassignment = 0) {

				std::string s = this->getAssignment(user, assignment, subassignment);

				rapidjson::Document doc;
				doc.Parse(s.c_str());
				if (doc.HasParseError())
					throw "Malformed JSON";

				try {
					std::string assignment_type = doc["assignment_type"].GetString();

					if (assignment_type != "ColorGrid")
						throw "Malformed ColorGrid JSON: Not a ColorGrid";
				}
				catch (rapidjson_exception re) {
					throw "Malformed JSON: Not a Bridges assignment?";
				}

				try {
					auto& data = doc["data"][0];

					std::string encoding = data["encoding"].GetString();
					if (encoding != "RAW" && encoding != "RLE")
						throw "Malformed ColorGrid JSON: encoding not supported";

					//Access doc["data"][0]["dimensions"]
					const auto& dimensions = data["dimensions"];
					int dimx = dimensions[0].GetInt();
					int dimy = dimensions[1].GetInt();

					if (debug())
						std::cerr << "Dimensions: " << dimx << "x" << dimy << std::endl;

					//Access doc["data"][0]["nodes"][0]
					std::string base64_encoded_assignment = data["nodes"][0].GetString();

					std::vector<bridges::BYTE> decoded = bridges::base64::decode(base64_encoded_assignment);

					bridges::ColorGrid cg (dimx, dimy);

					if (encoding == "RAW") {
						if (debug())
							std::cerr << "decoding RAW" << std::endl;
						if (debug())
							std::cerr << "length: " << decoded.size() << std::endl;
						if (decoded.size() < dimx * dimy * 4)
							throw "Malformed ColorGrid JSON: nodes is smaller than expected";

						//first pixel
						//std::cerr<<(int)decoded[0]<<" "<<(int)decoded[1]<<" "<<(int)decoded[2]<<" "<<(int)decoded[3]<<std::endl;

						//bridges::ColorGrid* ptr = new bridges::ColorGrid (dimx, dimy);

						size_t base = 0;

						for (int x = 0; x < dimx; ++x) {
							for (int y = 0; y < dimy; ++y) {
								bridges::Color c ((int)decoded[base],
									(int)decoded[base + 1],
									(int)decoded[base + 2],
									(int)decoded[base + 3]
								);

								cg.set(x, y, c);
								base += 4;
							}
						}
					}
					else if (encoding == "RLE") {
						if (debug())
							std::cerr << "Decoding RLE" << std::endl;

						int currentInDecoded = 0;
						int currentInCG = 0;
						while (currentInDecoded != decoded.size()) {
							if (currentInDecoded + 5 > decoded.size())
								throw "Malformed ColorGrid JSON: nodes is not a multiple of 5";

							int repeat = (BYTE) decoded[currentInDecoded++];
							int r = (BYTE) decoded[currentInDecoded++];
							int g = (BYTE) decoded[currentInDecoded++];
							int b = (BYTE) decoded[currentInDecoded++];
							int a = (BYTE) decoded[currentInDecoded++];

							if (debug())
								std::cerr << "indecoded: " << currentInDecoded
									<< " repeat: " << (int)repeat
									<< " color(" << (int)r << "," << (int)g << "," << (int)b << "," << (int)a << ")"
									<< std::endl;

							bridges::Color c (r, g, b, a);

							while (repeat >= 0) {
								int posX = currentInCG / dimy;
								int posY = currentInCG % dimy;
								if (posX >= dimx || posY >= dimy) {
									if (debug())
										std::cerr << posX << " " << dimx << " " << posY << " " << dimy << std::endl;
									throw "Malformed ColorGrid JSON: Too much data in nodes";
								}
								cg.set(posX, posY, c);

								currentInCG++;
								repeat --;
							}
						}
						if (debug())
							std::cerr << "written " << currentInCG << " pixels" << std::endl;
						if (currentInCG != dimx * dimy)
							throw "Malformed ColorGrid JSON: Not enough data in nodes";
					}

					return cg;
				}
				catch (rapidjson_exception re) {
					throw "Malformed ColorGrid JSON";
				}

			}
		private:
			/***
			 * This function obtains the JSON representation of a particular subassignment.
			 *
			 * @return a string that is the JSON representation of the subassignment as stored by the Bridges server.
			 * @param user the name of the user who uploaded the assignment
			 * @param assignment the ID of the assignment to get
			 * @param subassignment the ID of the subassignment to get
			 ***/
			std::string getAssignment(std::string user,
				int assignment,
				int subassignment = 0) {
				std::vector<std::string> headers;

				std::stringstream ss;

				///should probably get the server name from a Bridges object
				if (bridges_inst)
					ss << bridges_inst->getServerURL();
				else
					ss << bridges::Bridges::getDefaultServerURL();
				ss << "/assignmentJSON/"
					<< assignment << ".";
				ss << std::setfill('0') << std::setw(2) << subassignment;
				ss << "/" << user;

				std::string url = ss.str();

				std::string s = bridges::ServerComm::makeRequest(url, headers);

				return s;
			}

			void removeFirstOccurence (std::string & str, const std::string & toRemove) {
				size_t pos = str.find(toRemove);
				if (pos  != std::string::npos) {
					str.erase(pos, toRemove.length());
				}
			}

			///@brief This function returns the Movie and Actors playing
			///in them between two years
			///
			/// Internally this function gets directly the range data
			/// from wikidata. This can cause wikidata to kick the user
			/// out or return invalid JSON if the range is too wide.
			///
			/// @param yearbegin, yearend interval of years to obtain, yearbegin and yearend are included.
			/// @param vout vector where the pairs will be aded to
			void  getWikidataActorMovieDirect (int yearbegin, int yearend, std::vector<MovieActorWikidata>& vout) {
				std::string codename = "wikidata-actormovie-" + std::to_string(yearbegin) + "-" + std::to_string(yearend);
				std::string json;
				bool from_cache = false;
				try {
					if (my_cache.inCache(codename)) {
						json = my_cache.getDoc(codename);
						from_cache = true;
					}
				}
				catch (CacheException& ce) {
					//something went bad trying to access the cache
				  std::cout << "Exception while reading from cache. Ignoring cache and continue.\n( What was:"<<ce.what() <<")" << std::endl;
				}

				if (!from_cache) {
					std::vector<std::string> http_headers;
					http_headers.push_back("User-Agent: bridges-cxx"); //wikidata kicks you out if you don't have a useragent
					http_headers.push_back("Accept: application/json"); //tell wikidata we are OK with JSON

					string url = "https://query.wikidata.org/sparql?";

					//Q1860 is "English"
					//P364 is "original language of film or TV show"
					//P161 is "cast member"
					//P577 is "publication date"
					//A11424 is "film"
					//P31 is "instance of"
					// "instance of film" is necessary to filter out tv shows
					std::string sparqlquery =
						"SELECT ?movie ?movieLabel ?actor ?actorLabel WHERE \
{\
  ?movie wdt:P31 wd:Q11424.\
  ?movie wdt:P161 ?actor.\
  ?movie wdt:P364 wd:Q1860.\
  ?movie wdt:P577 ?date.\
  FILTER(YEAR(?date) >= " + std::to_string(yearbegin) + " && YEAR(?date) <= " + std::to_string(yearend) + ").\
    SERVICE wikibase:label { bd:serviceParam wikibase:language \"en\". } \
}";
					url += "query=" + ServerComm::encodeURLPart(sparqlquery);
					url += "&";
					url += "format=json";

					if (debug()) {
						std::cout << "URL: " << url << "\n";
					}
					// get the Wikidata json
					json = ServerComm::makeRequest(url, http_headers);

					try {
						my_cache.putDoc(codename, json);
					}
					catch (CacheException& ce) {
						//something went bad trying to access the cache
					  std::cerr << "Exception while storing in cache. Weird but not critical. (What was: "<<ce.what()<<" )" << std::endl;
					}
				}

				{
					using namespace rapidjson;
					rapidjson::Document doc;
					doc.Parse(json.c_str());
					if (doc.HasParseError())
						throw "Malformed JSON";

					try {
						const auto& resultsArray = doc["results"]["bindings"].GetArray();

						for (auto& mak_json : resultsArray) {
							MovieActorWikidata mak;

							// all wikidata uri start with "http://www.wikidata.org/entity/"
							// so strip it out because it does not help discriminate and
							// consume memory and runtime to compare string
							std::string actoruri = mak_json["actor"]["value"].GetString();
							std::string movieuri = mak_json["movie"]["value"].GetString();
							removeFirstOccurence (actoruri, "http://www.wikidata.org/entity/");

							removeFirstOccurence (movieuri, "http://www.wikidata.org/entity/");

							mak.setActorURI(actoruri);
							mak.setMovieURI(movieuri);
							mak.setActorName(mak_json["actorLabel"]["value"].GetString());
							mak.setMovieName(mak_json["movieLabel"]["value"].GetString());
							vout.push_back(mak);
						}

					}
					catch (rapidjson_exception re) {
						throw "Malformed JSON: Not from wikidata?";
					}
				}
			}
		public:

			///@brief This function returns the Movie and Actors playing
			///in them between two years.
			///
			/// Return movie pair in the [yearbegin; yearend] interval.
			///
			/// @param yearbegin first year to include
			/// @param yearend last year to include
			std::vector<MovieActorWikidata> getWikidataActorMovie (int yearbegin, int yearend) {
				//Internally this function get the data year by year. This
				//is pretty bad because it hits wikidata the first time
				//for multiple years. But it enables to work around
				//wikidata's time limit.  This also works well because the
				//Cache will store each year independently and so without
				//redundancy.  Though I (Erik) am not completely sure that a
				//movie can be appear in different years, for instance it
				//can be released in the US in 2005 but in canada in
				//2006...

				std::vector<MovieActorWikidata> ret;
				for (int y = yearbegin; y <= yearend; ++y) {
					cout << "getting year " << y << endl;
					getWikidataActorMovieDirect (y, y, ret);
				}
				return ret;
			}

			/**
			 * Returns ElevationData for the provided coordinate box at the
			 * given resolution. Note that the ElevationData that is returned
			 * may have slightly different location and resolution.
			 *
			 * @param latitMin minimum latitude requested
			 * @param longitMin maximum latitude requested
			 * @param latitMax minimum longitude requested
			 * @param longitMax maximum longitude requested
			 * @param res spatial resolution, aka the distance between two samples
			 * 		(in degrees)
			 **/


			/**
			 *	 @brief Parses the elevation data string and retuns
			 * 	   an Elevation object
			 *	@param elev_json  string containing the requested elevation data
			 *
			 *  @return Elevation data object
			 */


			/**
			* @brief retrieves the subreddits made available by BRIDGES
			*
			* @return a list of strings of subreddit names
			**/


			/**
			*     @brief retrieves the reddit posts from a subreddit
			*
			* @param subreddit the name of the subreddit ( check list available at http://bridges-data-server-reddit.bridgesuncc.org/list or using getAvailableSubreddits() )
			* @param time_request unix timestamp of when requested subreddit was generated or less than 0 for now
			*
			* @return a list of reddit objects with the data of the posts
			*
			**/


		private:
			/**
			 *   gets the hash code for the dataset
			 *
			 *   @param hash_url   url for hash code
			 *   @param data type  data set name
			 *
			 * 	 @return a hash code as a string or "false" if the hash value
			 *	 	doesnt exist on the server.
			 */
			string getHashCode (string hash_url, string data_type) {
				string hash_value;
				if (data_type == "osm" || data_type == "amenity" ||
					data_type == "elevation") {
					hash_value = ServerComm::makeRequest(hash_url, {"Accept: application/json"});
				}
				else if (data_type == "gutenberg")
					hash_value = hash_url;

				return hash_value;
			}

			/**
			 *  This method is a utility function that supports retrieving
			 *  external dataset given a url to the dataset's server as well
			 *	as a url to extract a hashcode for the dataset; the latter is
			 *  is to support local caching. The dataset is only retrieved
			 *  the server if a local copy is not available
			 *
			 *	Currently this function works with elevation, OpenStreet maps and
			 *  Amenity datasets
			 *
			 *
			 * Multiple dataset follow the same protocol. They have a hash url and a data url.
			 * Hitting the data URL should always returns the appropriate data.
			 * Hitting the hash URL should return a hash code for the data, assuming such a hash code is available.
			 *  If it is not available the cache url return "false".
			 *
			 * It is possible for the hash URL to return "false" if the underlying data is not know to the server yet.
			 * Internally, this happens in cases of server-side caching; for instance the first time a data is accessed, if you hit the hash URL before the data url, it shoudl return "false".
			 *
			 *
			 */
			std::string getDataSetJSON(std::string data_url, std::string hash_url,
				std::string data_type) {

				std::string data_json = "";

				// First check to see if the requested data is stored in local cache
				// get hash value for elevation data
				if (debug())
					cerr << "Checking the cache: Hash url: " << hash_url << "\n";

				// generate the hash code
				string hash_value = getHashCode(hash_url, data_type);

				bool dataloaded = false;

				if ((hash_value != "false") && (my_cache.inCache(hash_value) == true)) { //local cache contains the dataset
					try {
						data_json = my_cache.getDoc(hash_value);
						dataloaded = true;
					}
					catch (CacheException& ce) {
						//something went bad trying to access the data in the local cache
						std::cout << "Exception while reading from cache. "
							  << "Ignoring cache and continuing..\n (What was:"<<ce.what()<<")\n";
					}
				}
				if (!dataloaded) {
					//Data could not get accessed from cache for some reason.
					//So teh data need to be access from the remote server.
					//Then we will store it in a local cache for future usage.
					if (debug())
						std::cerr << "Hitting data URL: " << data_url << "\n";

					//Requests the data
					data_json = ServerComm::makeRequest(data_url,
					{"Accept: application/json"});

					//Store the data in cache for future reuse
					try {
						// We need the data's hash code to know where to store it in local cache.
						// We may already have it from the previous query.
						if (hash_value == "false") {
							if (debug())
								std::cerr << "Hitting hash URL: " << hash_value << "\n";

							hash_value = getHashCode(hash_url, data_type);
						}

						// This test should only ever be true if something wrong happens server-side
						if (hash_value == "false") {
							std::cerr << "Error while gathering hash value for " << data_type << " dataset..\n"
								<< "Weird but not critical.\n";
						}
						else {
							my_cache.putDoc(hash_value, data_json);
						}
					}
					catch (CacheException& ce) {
						//something went bad trying to access the cache
						std::cerr << "Exception while storing in cache. " <<
						  "Weird but not critical.\n" <<
						  "(What was: "<<ce.what()<<")\n";
						if (debug())
							std::cerr << "Tried to store hash=" << hash_value <<
								" key = " << data_json << std::endl;
					}
				}

				if (debug())
					cout << data_json;

				return data_json;
			}

	}; // class DataSource
} // namespace bridges
#endif
