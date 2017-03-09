<?hh // strict
namespace Usox\Sharesta;

interface RequestBodyInterface {

	public function getBody(): Map<string,mixed>;
}
