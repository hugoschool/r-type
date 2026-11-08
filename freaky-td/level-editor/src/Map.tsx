import type { Tile } from "./App";

type MapProps = {
    width: number;
    height: number;
    path: Tile[];
    setPath: any;
};

type TileInfo = {
    selected: boolean;
    first: boolean;
    last: boolean;
};

export default function Map({ width, height, path, setPath }: MapProps) {
    const selectTile = (x: number, y: number) => {
        if (getTileInfo(x, y).selected) {
            for (let i = 0; i < path.length; i++) {
                const tile = path[i];
                if (tile.x === x && tile.y === y) {
                    path.splice(i, 1);
                }
            }
            setPath([...path]);
        } else {
            setPath([...path, { x, y }]);
        }
    };

    const getTileInfo = (x: number, y: number): TileInfo => {
        let selected = false;
        let first = false;
        let last = false;

        for (let i = 0; i < path.length; i++) {
            const tile = path[i];

            if (tile.x === x && tile.y === y) {
                selected = true;

                if (i === 0) first = true;
                if (i === path.length - 1) last = true;
            }
        }
        return { selected, first, last };
    };

    // I STILL know what I'm doing
    // oxlint-disable react/immutability
    const createTable = () => {
        let rows = [];
        for (let y = 0; y < height; y++) {
            let col = [];

            for (let x = 0; x < width; x++) {
                const tileInfo = getTileInfo(x, y);

                col.push(
                    <td
                        className={tileInfo.selected ? "selected" : ""}
                        key={"x-" + x}
                        onClick={() => selectTile(x, y)}
                    >
                        <span>{tileInfo.first ? "First" : ""}</span>
                        <br />
                        <span>{tileInfo.last ? "Last" : ""}</span>
                    </td>,
                );
            }

            rows.push(<tr key={"y-" + y}>{col}</tr>);
        }
        return rows;
    };

    return (
        <>
            <table>
                <tbody>{createTable()}</tbody>
            </table>
        </>
    );
}
