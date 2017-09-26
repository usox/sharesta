<?hh // strict
namespace Usox\Sharesta;

interface RequestBodyInterface {

	public function getBody(): Map<string,mixed>;

	public function getAsString(string $key): string;

	public function getAsInt(string $key): int;

	public function getAsBool(string $key): bool;

	public function getAsVector(string $key): Vector<mixed>;

	public function getAsMap(string $key): Map<mixed, mixed>;
}
