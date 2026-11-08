import { useState } from "react";
import "./App.css";
import Map from "./Map";

export type Tile = {
    x: number;
    y: number;
};

type Entity = {
    total_amount: number;
    spawn_rate: number;
    types: string[];
};

type Player = {
    health_points: number;
};

type Level = {
    paths: Tile[][];
    entities: Entity;
    player: Player;
};

function defaultLevel(): Level {
    return {
        paths: [[]],
        entities: {
            total_amount: 0,
            spawn_rate: 0,
            types: ["shooter"],
        },
        player: {
            health_points: 0,
        },
    };
}

function App() {
    const [levels, setLevels] = useState<Level[]>([defaultLevel()]);
    const [levelIndex, setLevelIndex] = useState(0);

    const newLevel = () => {
        setLevels([...levels, defaultLevel()]);
    };

    const loadLevels = (e: React.ChangeEvent<HTMLInputElement>) => {
        const prompt = confirm("This will delete everything currently save, are you sure?");
        if (!prompt) {
            return;
        }

        if (!e.target.files || e.target.files.length < 1) {
            return;
        }

        const file = e.target.files[0];

        const reader = new FileReader();
        reader.readAsText(file, "UTF-8");
        reader.onload = function ({ target }) {
            if (!target) {
                return;
            }

            const content = target.result?.toString();
            if (!content) {
                return;
            }

            console.log(content);

            let parsedLevels: Level[] = JSON.parse(content);

            setLevels([...parsedLevels]);
        };
        reader.onerror = function () {
            console.error("Couldn't read file :-(");
        };
    };

    // https://cloudinary.com/blog/questions/how-to-save-a-file-in-javascript#browser_trigger_a_download_with_blob_a_download_
    const saveLevels = () => {
        const blob = new Blob([JSON.stringify(levels, null, 2)], { type: "application/json" });
        const url = URL.createObjectURL(blob);

        const a = document.createElement("a");
        a.href = url;
        a.download = "levels.json";
        document.body.appendChild(a);
        a.click();
        a.remove();
        URL.revokeObjectURL(url);
    };

    const saveCurrentLevel = (newLevel: Level) => {
        setLevels((prevLevels) => [
            ...prevLevels.slice(0, levelIndex),
            newLevel,
            ...prevLevels.slice(levelIndex + 1),
        ]);
    };

    // I know what I'm doing...
    // oxlint-disable react/immutability

    const setTotalAmount = (nb: number) => {
        let level = levels[levelIndex];
        level.entities.total_amount = nb;

        saveCurrentLevel(level);
    };

    const setSpawnRate = (nb: number) => {
        let level = levels[levelIndex];
        level.entities.spawn_rate = nb;

        saveCurrentLevel(level);
    };

    const setHP = (nb: number) => {
        let level = levels[levelIndex];
        level.player.health_points = nb;

        saveCurrentLevel(level);
    };

    const setPath = (path: Tile[]) => {
        let level = levels[levelIndex];
        level.paths[0] = path;

        saveCurrentLevel(level);
    };

    return (
        <>
            <div>
                <label htmlFor="load_file">Load file: </label>
                <input
                    type="file"
                    id="load_file"
                    name="load_file"
                    accept="application/json"
                    onChange={loadLevels}
                />
            </div>
            <div>
                <label htmlFor="save_file">Save file: </label>
                <button id="save_file" name="save_file" onClick={saveLevels}>
                    Save file
                </button>
            </div>
            <div>
                <label htmlFor="level_index">Level index (levels amount {levels.length}): </label>
                <input
                    type="number"
                    id="level_index"
                    name="level_index"
                    min={0}
                    max={levels.length - 1}
                    value={levelIndex}
                    onChange={(e) => setLevelIndex(Number(e.target.value))}
                />
            </div>
            <button onClick={newLevel}>New level</button>
            <div>
                <h1>Entities</h1>
                <div>
                    <label htmlFor="total_amount">Total amount: </label>
                    <input
                        type="number"
                        id="total_amount"
                        name="total_amount"
                        min={0}
                        value={levels[levelIndex].entities.total_amount}
                        onChange={(e) => setTotalAmount(Number(e.target.value))}
                    />
                </div>
                <div>
                    <label htmlFor="spawn_rate">Spawn rate (entities per second): </label>
                    <input
                        type="number"
                        id="spawn_rate"
                        name="spawn_rate"
                        min={0}
                        value={levels[levelIndex].entities.spawn_rate}
                        onChange={(e) => setSpawnRate(Number(e.target.value))}
                    />
                </div>
            </div>
            <div>
                <h1>Player</h1>
                <div>
                    <label htmlFor="health_points">HP: </label>
                    <input
                        type="number"
                        id="health_points"
                        name="health_points"
                        min={0}
                        value={levels[levelIndex].player.health_points}
                        onChange={(e) => setHP(Number(e.target.value))}
                    />
                </div>
            </div>
            <div>
                <h1>Paths</h1>
                <Map
                    width={20}
                    height={16}
                    path={levels[levelIndex].paths[0]}
                    setPath={setPath}
                ></Map>
            </div>
        </>
    );
}

export default App;
